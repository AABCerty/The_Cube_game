#include "boost_tcp_client.h"
#include "Server_request.h"
#include "Global_var.h"

// boost
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/asio/read_until.hpp>
// STL
#include <iostream>
#include <vector>
#include <sstream>

// boost ASIO
using namespace boost::asio;

#define MEM_FN(x) boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y) boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z) boost::bind(&self_type::x, shared_from_this(),y,z)


talk_to_server::talk_to_server(io_service& service, const std::string& username, std::vector<Cube*>& tubes) :  m_sock(service), b_started(true), s_username(username), m_timer(service), m_cubes(tubes) {
    std::cout << "Constructor" << std::endl;
}

void talk_to_server::start(ip::tcp::endpoint ep) {
    std::cout << "start" << std::endl;
    m_sock.async_connect(ep, MEM_FN1(on_connect, _1));
}

typedef boost::system::error_code error_code;
typedef boost::shared_ptr<talk_to_server> ptr;

ptr talk_to_server::start(io_service& service, ip::tcp::endpoint ep, const std::string& username, std::vector<Cube*>& tubes) {
    ptr new_talk_to_server(new talk_to_server(service, username, tubes));
    new_talk_to_server->start(ep);
    return new_talk_to_server;
}

void talk_to_server::stop() {
    if (!b_started) {
        return;
    }
    std::cout << "stopping " << s_username << std::endl;
    b_started = false;
    m_sock.close();
}

bool talk_to_server::started() {
    return b_started;
}

void talk_to_server::on_connect(const error_code& err) {
    std::cout << "Connected" << std::endl;
    if (!err) {
        do_write(EnumToString(Request::LOGIN) + s_username + "\n");
    } else {
        stop();
    }
}

void talk_to_server::on_move(const char ch) {
    if (ch == EnumToChar(Request::X_PLUS_Y_PLUS)) {
        for (auto& x : m_cubes) {
            x->Move(Request::X_PLUS_Y_PLUS);
        }
    } else if (ch == EnumToChar(Request::X_PLUS_Y_MINUS)) {
        for (auto& x : m_cubes) {
            x->Move(Request::X_PLUS_Y_MINUS);
        }
    } else if (ch == EnumToChar(Request::X_MINUS_Y_PLUS)) {
        for (auto& x : m_cubes) {
            x->Move(Request::X_MINUS_Y_PLUS);
        }
    } else if (ch == EnumToChar(Request::X_MINUS_Y_MINUS)) {
        for (auto& x : m_cubes) {
            x->Move(Request::X_MINUS_Y_MINUS);
        }
    } else if (ch == EnumToChar(Request::X_PLUS)) {
        for (auto& x : m_cubes) {
            x->Move(Request::X_PLUS);
        }
    } else if (ch == EnumToChar(Request::X_MINUS)) {
        for (auto& x : m_cubes) {
            x->Move(Request::X_MINUS);
        }
    } else if (ch == EnumToChar(Request::Y_PLUS)) {
        for (auto& x : m_cubes) {
            x->Move(Request::Y_PLUS);
        }
    } else if (ch == EnumToChar(Request::Y_MINUS)) {
        for (auto& x : m_cubes) {
            x->Move(Request::Y_MINUS);
        }
    }
    do_read_enum();
}

void talk_to_server::on_read(const error_code& err, size_t bytes) {
    if (err) {
        stop();
    }
    if (!started()) {
        return;
    }
    std::string msg(read_buffer, bytes);
    std::cout << "MSG: " << msg << std::endl;
    for (const auto& ch : msg) {
        if (ch == EnumToChar(Request::LOGIN_OK)) {
            on_login();
        } else if (ch == EnumToChar(Request::NEXT_LEVEL)) {
            NextLevel();
        } else if (ch == EnumToChar(Request::RESTART)) {
            Restart();
        } else {
            on_move(ch);
        }
    }
}

void talk_to_server::on_login() {
    std::cout << "Login OK" << std::endl;
    do_read_enum();
}

void talk_to_server::on_write(const error_code& err, size_t bytes) {
    if (err) {
        return;
    }
    std::cout << bytes << std::endl;
    do_read_enum();
}
void talk_to_server::do_read() {
    async_read(m_sock, buffer(read_buffer), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
}
void talk_to_server::do_read_enum() {
        m_sock.async_read_some(buffer(read_buffer), MEM_FN2(on_read, _1, _2));
}
void talk_to_server::do_write(const std::string& msg) {
    if (!b_started) {
        return;
    }
    std::copy(msg.begin(), msg.end(), write_buffer);
    m_sock.async_write_some(buffer(write_buffer, msg.size()), MEM_FN2(on_write, _1, _2));
}
void talk_to_server::do_write_enum(Request request) {
    if (!b_started) {
        return;
    }
    m_sock.async_write_some(buffer(EnumToString(request)), MEM_FN2(on_write, _1, _2));
}

size_t talk_to_server::read_complete(const boost::system::error_code& err, size_t bytes) {
    if (err) {
        return 0;
    }
    bool find = std::find(read_buffer, read_buffer+bytes, '\n') < read_buffer + bytes;
    return find ? 0 : 1;
}









