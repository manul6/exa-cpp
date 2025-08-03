#include "exa/client.h"

namespace exa {
    Client::Client(
        boost::asio::io_context& io_context,
        std::string_view api_key,
        std::string_view host,
        short port
    )
        : api_key_(api_key),
          host_(host),
          port_(std::to_string(port)),
          ssl_ctx_(boost::asio::ssl::context::tlsv12),
          stream_(io_context, ssl_ctx_),
          resolver_(io_context),
          endpoint_(boost::asio::ip::tcp::v4(), port) {
        ssl_ctx_.set_default_verify_paths();
    }   

    boost::asio::awaitable<void> Client::connect() {
        auto endpoints = co_await resolver_.async_resolve(host_, port_, boost::asio::use_awaitable);
        co_await boost::beast::get_lowest_layer(stream_).async_connect(endpoints, boost::asio::use_awaitable);
        co_await stream_.async_handshake(boost::asio::ssl::stream_base::client, boost::asio::use_awaitable);
    }

    boost::asio::awaitable<std::string> Client::search(std::string_view query) {
        co_await connect();

        boost::beast::http::request<boost::beast::http::string_body> req;
        req.method(boost::beast::http::verb::get);
        req.target("/search?q=" + std::string(query));
        req.version(11);
        req.set(boost::beast::http::field::host, host_);
        req.set(boost::beast::http::field::user_agent, "exa-cpp-client");
        req.set(boost::beast::http::field::accept, "application/json");
        req.set(boost::beast::http::field::authorization, "Bearer " + api_key_);

        co_await boost::beast::http::async_write(stream_, req, boost::asio::use_awaitable);

        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::string_body> res;
        co_await boost::beast::http::async_read(stream_, buffer, res, boost::asio::use_awaitable);

        if (res.result_int() != 200) {
            throw std::runtime_error("Failed to search: " + res.body());
        }

        co_return res.body();
    }

    Client::~Client() {
        boost::beast::get_lowest_layer(stream_).close();
    }
}