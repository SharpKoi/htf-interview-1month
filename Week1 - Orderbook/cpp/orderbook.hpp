#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include <map>
#include <thread>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>

namespace net = boost::asio;
namespace ssl = net::ssl;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = net::ip::tcp;
using json = nlohmann::json;


class OrderBook {
private:
    net::io_context ioc;
    ssl::context ctx;
    websocket::stream<ssl::stream<tcp::socket>> ws;

    std::map<double, double, std::greater<double>> bids; // 買單 (price -> size)
    std::map<double, double, std::less<double>> asks; // 賣單 (price -> size)

    // thread safety
    std::queue<std::string> msgQueue;
    std::mutex queueMutex;
    std::condition_variable queueCond;

    std::shared_mutex orderbookMutex;

public:
    OrderBook(std::string ws_url, std::string endpoint, std::string symbol, int depth = 1)
        : ctx(ssl::context::tls_client), ws(ioc, ctx) {
        setupConnection(ws_url, endpoint);
        subscribe(std::format("orderbook.{}.{}", depth, symbol));

        std::thread(&OrderBook::readMessage, this).detach();  // Producer
        std::thread(&OrderBook::processMessage, this).detach();  // Consumer
    }

    void setupConnection(std::string ws_url, std::string endpoint) {
        ctx.set_verify_mode(ssl::verify_peer);
        ctx.set_default_verify_paths();

        ws.next_layer().set_verify_mode(ssl::verify_peer);

        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(ws_url, "443");
        net::connect(ws.next_layer().next_layer(), results);
        std::cout << "✅ TCP Connection successful!" << std::endl;

        // Config SNI
        if(!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), ws_url.data())) {
            throw beast::system_error(
                beast::error_code(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()),
                "Failed to set SNI hostname"
            );
        }

        // SSL handshake
        ws.next_layer().handshake(ssl::stream_base::client);
        std::cout << "✅ SSL Handshake successful!" << std::endl;

        // Websocket handshake
        ws.handshake(ws_url, endpoint);
        std::cout << "✅ Websocket Handshake successful!" << std::endl;
    }

    void subscribe(std::string topic) {
        std::string subscribe_msg = std::format(R"({{
            "op": "subscribe",
            "args": ["{}"]
        }})", topic);
        ws.write(net::buffer(subscribe_msg));

        beast::flat_buffer buffer;

        // handle the welcome msg
        ws.read(buffer);
        std::string msg = beast::buffers_to_string(buffer.data());
        json response = json::parse(msg);
        if(response["success"]) {
            std::cout << std::format("✅ Subscribed topic: {} !", topic) << std::endl;
        }
        buffer.consume(buffer.size());
    }

    void readMessage() {
        beast::flat_buffer buffer;
        while (true) {
            ws.read(buffer);
            std::string msg = beast::buffers_to_string(buffer.data());
            
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                msgQueue.push(msg);
            }

            // Notify the variable condition to check the predicate.
            queueCond.notify_one();
            buffer.consume(buffer.size());
        }
    }

    void processMessage() {
        while(true) {
            std::string msg;

            {
                std::unique_lock<std::mutex> lock(queueMutex);
                // wait(lock, predicate): pause the current thread until the predicate returns true.
                // The predicate is triggered everytime the condition variable is notified.
                // `this`, which is the class instance, is captured in predicate so that the msgQueue can be accessed.
                queueCond.wait(lock, [this]() { return !msgQueue.empty(); });

                msg = msgQueue.front();
                msgQueue.pop();
            }

            handleMessage(msg);
        }
    }

    void handleMessage(const std::string& message) {
        json response = json::parse(message);

        for(const auto& entry : response["data"]["b"]) {
            double price = std::stod(entry[0].get<std::string>());
            double size = std::stod(entry[1].get<std::string>());

            updateBids(price, size);
        }

        for(const auto& entry : response["data"]["a"]) {
            double price = std::stod(entry[0].get<std::string>());
            double size = std::stod(entry[1].get<std::string>());

            updateAsks(price, size);
        }
    }
    
    void updateBids(double price, double size) {
        std::unique_lock<std::shared_mutex> lock(orderbookMutex);
        if(size == 0) {
            bids.erase(price);
        }else if(size > 0) {
            bids[price] = size;
        }
    }

    void updateAsks(double price, double size) {
        std::unique_lock<std::shared_mutex> lock(orderbookMutex);
        if(size == 0) {
            asks.erase(price);
        }else if(size > 0) {
            asks[price] = size;
        }
    }

    void print() {
        std::shared_lock<std::shared_mutex> lock(orderbookMutex);
        for(const auto& [price, size] : asks) {
            std::cout << "[SELL] " << price << "> " << size << std::endl;
        }
        for(const auto& [price, size] : bids) {
            std::cout << "[BUY]  " << price << "> " << size << std::endl;
        }
    }
};
