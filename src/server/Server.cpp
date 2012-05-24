#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include "../client/Connection.h"   // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include "../db/DataBase.h"

typedef boost::shared_ptr<Connection> connection_ptr;

class Server {
  public:
    /// Constructor opens the acceptor and starts waiting for the first incoming
    /// connection.
    Server(boost::asio::io_service & io_service,
           unsigned short port):acceptor_(io_service,
                                          boost::asio::ip::tcp::
                                          endpoint(boost::asio::ip::tcp::v4(),
                                                   port)),
        db_(new DataBase()) {
        // Initialize the database
        db_->initialize();
        // Start an accept operation for a new connection.
        connection_ptr new_conn(new Connection(acceptor_.get_io_service()));
          acceptor_.async_accept(new_conn->socket(),
                                 boost::bind(&server::handle_accept, this,
                                             boost::asio::placeholders::error,
                                             new_conn));
    }
    /// Handle completion of a accept operation.
        void handle_accept(const boost::system::error_code & e,
                           connection_ptr conn) {
        if (!e) {
            // Successfully accepted a new connection. Get the queries and
            // send result back to the client.
            // The connection::async_write() function will automatically
            // serialize the data structure for us.
            conn->async_write("",
                              boost::bind(&server::handle_write, this,
                                          boost::asio::placeholders::error,
                                          conn));
        }
    }
    /// Handle completion of a write operation.
    void handle_write(const boost::system::error_code & e,
                      connection_ptr conn) {
        // Start an accept operation for a new connection.
        connection_ptr new_conn(new Connection(acceptor_.get_io_service()));
        acceptor_.async_accept(new_conn->socket(),
                               boost::bind(&server::handle_accept, this,
                                           boost::asio::placeholders::error,
                                           new_conn));

    }

  private:
    /// The acceptor object used to accept incoming socket connections.
    boost::asio::ip::tcp::acceptor acceptor_;
    DataBase *db_;
};

int main(int argc, char *argv[]) {
    try {
        // Check command line arguments.
        if (argc != 2) {
            std::cerr << "Usage: server <port>" << std::endl;
            return 1;
        }
        unsigned short port = boost::lexical_cast < unsigned short >(argv[1]);

        boost::asio::io_service io_service;
        Server server(io_service, port);
        io_service.run();
    }
    catch(std::exception & e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
