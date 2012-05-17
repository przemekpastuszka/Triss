/*
* Copyright 2012 Michał Rychlik
*/
#ifndef TRISS_SRC_CLIENT_CONNECTION_H_
#define TRISS_SRC_CLIENT_CONNECTION_H_

#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>
#include <vector>

class Connection {
private:
    Connection(const Connection& c);
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket_;
    enum {
        header_length = 8
    };
    std::string outbound_header_;
    std::string outbound_data_;
    char inbound_header_[header_length];
    std::vector<char> inbound_data_;
public:
    Connection(const std::string& host, const std::string& port);
    boost::asio::ip::tcp::socket& socket();
    void handle_connect(
        const boost::system::error_code& e,
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator
    );
    template <typename T, typename Handler> void async_write(const T& t,
                                                             Handler handler);
    template <typename T, typename Handler> void async_read(T& t,
                                                            Handler handler);
    template <typename T, typename Handler> void handle_read_header(
        const boost::system::error_code& e, T& t, boost::tuple<Handler> handler
    );
    template <typename T, typename Handler> void handle_read_data(
        const boost::system::error_code& e, T& t, boost::tuple<Handler> handler
    );
    void run_io_service();
};

#endif /* TRISS_SRC_CLIENT_CONNECTION_H_ */
