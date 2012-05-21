/*
* Copyright 2012 Michał Rychlik
*/

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/bind.hpp>
#include <sstream>

#include "Connection.h"

Connection::Connection(const std::string& host,
                       const std::string& port) : socket_(io_service) {
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(host, port);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
            resolver.resolve(query);
    boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
    this->socket().async_connect(endpoint,
                                 boost::bind(
                                     &Connection::handle_connect, this,
                                     boost::asio::placeholders::error,
                                     ++endpoint_iterator
                                 )
    );
}

boost::asio::ip::tcp::socket& Connection::socket() {
    return socket_;
}

void Connection::handle_connect(
    const boost::system::error_code& e,
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator
) {
    if (e) {
        if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator()) {
            this->socket().close();
            boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
            this->socket().async_connect(
                endpoint,
                boost::bind(&Connection::handle_connect, this,
                            boost::asio::placeholders::error,
                            ++endpoint_iterator
                )
            );
        } else {
            std::cerr << e.message() << std::endl;
        }
    }
}

template <typename T, typename Handler> void Connection::async_write(
    const T& t, Handler handler
) {
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << t;
    outbound_data_ = archive_stream.str();
    std::ostringstream header_stream;
    header_stream << std::setw(header_length) << std::hex <<
        outbound_data_.size();
    if (!header_stream || header_stream.str().size() != header_length) {
        boost::system::error_code error(boost::asio::error::invalid_argument);
        socket_.io_service().post(boost::bind(handler, error));
        return;
    }
    outbound_header_ = header_stream.str();
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(outbound_header_));
    buffers.push_back(boost::asio::buffer(outbound_data_));
    boost::asio::async_write(socket_, buffers, handler);    
}

template <typename T, typename Handler> void Connection::async_read(
    T& t, Handler handler) {
    void (Connection::*f)(
        const boost::system::error_code&, T&, boost::tuple<Handler>
    ) = &Connection::handle_read_header<T, Handler>;
    boost::asio::async_read(socket_, boost::asio::buffer(inbound_header_),
                            boost::bind(f, this,
                                        boost::asio::placeholders::error,
                                        boost::ref(t),
                                        boost::make_tuple(handler)
                            )
    );
}

template <typename T, typename Handler> void Connection::handle_read_header(
    const boost::system::error_code& e, T& t, boost::tuple<Handler> handler
) {
    if (e) {
        boost::get<0>(handler)(e);
    } else {
        std::istringstream is(std::string(inbound_header_, header_length));
        std::size_t inbound_data_size = 0;
        if (!(is >> std::hex >> inbound_data_size)) {
            boost::system::error_code error(
                boost::asio::error::invalid_argument
            );
            boost::get<0>(handler)(error);
            return;
        }
        inbound_data_.resize(inbound_data_size);
        void (Connection::*f)(
            const boost::system::error_code&, T&, boost::tuple<Handler>
        ) = &Connection::handle_read_data<T, Handler>;
        boost::asio::async_read(socket_, boost::asio::buffer(inbound_data_),
                                boost::bind(f, this,
                                            boost::asio::placeholders::error,
                                            boost::ref(t), handler
                                )
        );
    }
}

template <typename T, typename Handler> void Connection::handle_read_data(
    const boost::system::error_code& e, T& t, boost::tuple<Handler> handler
) {
    if (e) {
        boost::get<0>(handler)(e);
    } else {
        try {
            std::string archive_data(&inbound_data_[0], inbound_data_.size());
            std::istringstream archive_stream(archive_data);
            boost::archive::text_iarchive archive(archive_stream);
            archive >> t;
        } catch (std::exception& e) {
            boost::system::error_code error(
                boost::asio::error::invalid_argument
            );
            boost::get<0>(handler)(error);
            return;
        }
        boost::get<0>(handler)(e);
    }
}

void Connection::run_io_service() {
    io_service.run();
}

