#pragma once

#include "Server_request.h"
#include "Cube.h"

// boost
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/core/noncopyable.hpp>

// STL
#include <iostream>
#include <vector>
#include <sstream>

// boost ASIO
using namespace boost::asio;

class talk_to_server : public boost::enable_shared_from_this<talk_to_server>, boost::noncopyable
{
private:
    typedef talk_to_server self_type;
    talk_to_server(io_service& service, const std::string& username, std::vector<Cube*>& tubes);
    void start(ip::tcp::endpoint ep);

public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_server> ptr;

    static ptr start(io_service& service, ip::tcp::endpoint ep, const std::string& username, std::vector<Cube*>& tubes);
    void stop();
    bool started();

    ip::tcp::socket m_sock;
    void do_write(const std::string& msg);
    void do_write_enum(Request request);

private:
    void on_connect(const error_code& err);

    void on_read(const error_code& err, size_t bytes);
    void on_login();

    void on_move(const char ch);

    void on_write(const error_code& err, size_t bytes);
    void do_read();
    void do_read_enum();

    size_t read_complete(const boost::system::error_code& err, size_t bytes);


    enum {max_msg = 1024};
    char read_buffer[max_msg];
    char write_buffer[max_msg];
    bool b_started;
    std::string s_username;
    deadline_timer m_timer;
    std::vector<Cube*> m_cubes;

};

