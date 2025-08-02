#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

boost::asio::awaitable<void> search_exa(boost::asio::io_context& io_context) {
    boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::tlsv12);
    ssl_ctx.set_default_verify_paths();
    
    boost::beast::ssl_stream<boost::beast::tcp_stream> stream(io_context, ssl_ctx);
    
    boost::asio::ip::tcp::resolver resolver(io_context);
    auto endpoints = co_await resolver.async_resolve("api.exa.ai", "443", boost::asio::use_awaitable);
    
    co_await boost::beast::get_lowest_layer(stream).async_connect(endpoints, boost::asio::use_awaitable);
    co_await stream.async_handshake(boost::asio::ssl::stream_base::client, boost::asio::use_awaitable);
    
    std::cout << "connected to exa api" << std::endl;
    
    boost::beast::http::request<boost::beast::http::string_body> req;
    req.method(boost::beast::http::verb::get);
    req.target("/search?q=c%2B%2B%20networking");
    req.version(11);
    req.set(boost::beast::http::field::host, "api.exa.ai");
    req.set(boost::beast::http::field::user_agent, "exa-cpp-poc");
    req.set(boost::beast::http::field::accept, "application/json");
    
    co_await boost::beast::http::async_write(stream, req, boost::asio::use_awaitable);
    
    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::string_body> res;
    co_await boost::beast::http::async_read(stream, buffer, res, boost::asio::use_awaitable);
    
    std::cout << "exa search response status: " << res.result_int() << std::endl;
    std::cout << "response body: " << res.body().substr(0, 300) << "..." << std::endl;
}

int main() {
    boost::asio::io_context io_context;
    
    boost::asio::co_spawn(io_context, search_exa(io_context), boost::asio::detached);
    
    io_context.run();
    return 0;
}
