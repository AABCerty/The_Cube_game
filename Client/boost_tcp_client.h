#pragma once

#include "Server_request.h"
#include "Cube.h"
#include "Menu.h"

// boost
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/core/noncopyable.hpp>

// STL
#include <vector>

// boost ASIO
using namespace boost::asio;

class talk_to_server : public boost::enable_shared_from_this<talk_to_server>, boost::noncopyable
{
private:
    typedef talk_to_server self_type;

    talk_to_server();
    void start(ip::tcp::endpoint ep);

public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_server> ptr;

    static ptr start1(ip::tcp::endpoint ep);
    void stop();
    bool started();

    ip::tcp::socket m_sock;

    void do_write(const std::string& msg);
    void do_write(const Request request);
    void do_write(const Request request, const std::string& msg);
    void foo();

private:
    void on_connect(const error_code& err);
    void on_read(const error_code& err, size_t bytes);
    void on_login();
    void on_move(const std::string& str);
    void on_write(const error_code& err, size_t bytes);
    void do_read();
    void do_read_ONCE();

    size_t read_complete(const boost::system::error_code& err, size_t bytes);

    enum {max_msg = 1024};
    char read_buffer[max_msg];
    char write_buffer[max_msg];
    bool b_started;
    friend void Text_Enter_Login::Text();
    //friend Menu;
    std::string s_username;
    deadline_timer m_timer;
};

