#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "../serialization/serialization.h"

using boost::asio::ip::tcp;
using namespace std;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_service& io_service)
    {
        return pointer(new tcp_connection(io_service));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        cout << "Starting" << endl;

        // Приймаємо задачу
        boost::array<char, 10000> buf;
        boost::system::error_code error;

        size_t len = socket_.read_some(boost::asio::buffer(buf), error);
        cout << " Server asio received " << len << endl;

        // Тут ми десеріалізуємо прийняті дані
        string message;
        copy(buf.begin(), buf.begin()+buf.size(), back_inserter(message));
        _data Data = deserialize(message);

        cout << " Server decoded and searched substring " << Data.Sub << endl;

        // Тут ми відправляємо відповідь
        string response = to_string(myCount(Data.Sub, Data.Full)) + '\n';
        cout << " Server send back answer=" << response << endl;

        boost::asio::async_write(socket_, boost::asio::buffer(response),
                                 boost::bind(&tcp_connection::handle_write,
                                             shared_from_this(), boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }

private:
    tcp_connection(boost::asio::io_service& io_service) : socket_(io_service) {}
    void handle_write(const boost::system::error_code&, size_t) {}

    tcp::socket socket_;
};

class tcp_server
{
public:
    tcp_server(boost::asio::io_service& io_service)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    {
        start_accept(); // Asynchronous
    }

private:
    int port = 14;
    void start_accept()
    {
        cout << "Starting accept" << endl;
        // creates a socket
        tcp_connection::pointer new_connection =
                tcp_connection::create(acceptor_.get_io_service());
        acceptor_.async_accept(new_connection->socket(),
                               boost::bind(&tcp_server::handle_accept, this, new_connection,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code& error)
    {
        if (!error)
            new_connection->start();

        start_accept(); // Resume listening
    }

    tcp::acceptor acceptor_;
};

int main()
{
    try
    {
        boost::asio::io_service io_service;
        tcp_server server(io_service);
        io_service.run();
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}