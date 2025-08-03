#ifndef EXA_CLIENT_H
#define EXA_CLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

namespace exa {
    class Client {
        public:
            Client(
                boost::asio::io_context& io_context,
                std::string_view api_key,
                std::string_view host = "api.exa.ai",
                short port = 443
            );
            ~Client();
            boost::asio::awaitable<void> connect();
            boost::asio::awaitable<std::string> search(std::string_view query);
        private:
            std::string api_key_;
            std::string host_;
            std::string port_;
            boost::asio::ssl::context ssl_ctx_;
            boost::beast::ssl_stream<boost::beast::tcp_stream> stream_;
            boost::asio::ip::tcp::resolver resolver_;
            boost::asio::ip::tcp::endpoint endpoint_;
            boost::beast::flat_buffer buffer_;
    };
}

#endif