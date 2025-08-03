#include <iostream>
#include <string>

#include "exa/client.h"

int main() {
    boost::asio::io_context io_context;

    // note: replace with actual exa api key for real usage
    exa::Client client(io_context, "sk-exa-1234567890abcdef");
    
    boost::asio::co_spawn(io_context, 
        [&client]() -> boost::asio::awaitable<void> {
            try {
                std::cout << "searching for 'c++ networking'..." << std::endl;
                std::string result = co_await client.search("c++ networking");
                std::cout << "search successful!" << std::endl;
                std::cout << "result preview: " << result.substr(0, 300) << "..." << std::endl;
            } catch (const std::exception& e) {
                std::cout << "search failed: " << e.what() << std::endl;
            }
        }, 
        boost::asio::detached);
    
    io_context.run();
    return 0;
}
