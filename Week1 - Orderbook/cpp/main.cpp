#include "orderbook.hpp"
#include <iostream>
#include <thread>  
#include <chrono>

namespace net = boost::asio;
namespace ssl = net::ssl;
namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = net::ip::tcp;


const std::string BYBIT_WS_HOST = "stream.bybit.com";


int main() {
    try {
        OrderBook orderbook = OrderBook(BYBIT_WS_HOST, "/v5/public/spot", "BTCUSDT", 50);
        while(true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            orderbook.print();
            std::cout << "--- --- --- --- ---" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
