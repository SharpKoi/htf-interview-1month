cmake_minimum_required(VERSION 3.20)
project(BybitOrderbook)

set(CMAKE_CXX_STANDARD 23)

set(BOOST_ROOT "/opt/boost")
set(BOOST_INCLUDEDIR "/opt/boost/include")
set(BOOST_LIBRARYDIR "/opt/boost/lib")
find_package(Boost REQUIRED COMPONENTS system thread chrono log)
find_package(OpenSSL REQUIRED)
find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(orderbook main.cpp)

include_directories("/opt/boost/include")
include_directories(${OPENSSL_INCLUDE_DIR})

target_include_directories(orderbook PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(orderbook PRIVATE 
    pthread 
    ${OPENSSL_LIBRARIES} 
    Boost::system 
    Boost::thread 
    Boost::chrono 
    Boost::log 
    Arrow::arrow_shared 
    Parquet::parquet_shared
    nlohmann_json::nlohmann_json
)
