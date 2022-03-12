#include "boost_tcp_client.h"
#include "Server_request.h"
#include "Global_var.h"

// boost
#include <boost/bind.hpp>

// STL
#include <iostream>

// boost ASIO
using namespace boost::asio;

#define MEM_FN(x) boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y) boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z) boost::bind(&self_type::x, shared_from_this(),y,z)


 talk_to_server::talk_to_server() :  m_sock(service), b_started(true), m_timer(service) {
    std::cout << "Constructor" << std::endl;
}

void talk_to_server::start(ip::tcp::endpoint ep) {
    m_sock.async_connect(ep, MEM_FN1(on_connect, _1));
}

typedef boost::system::error_code error_code;
typedef boost::shared_ptr<talk_to_server> ptr;

ptr talk_to_server::start1(ip::tcp::endpoint ep) {
    ptr new_talk_to_server(new talk_to_server());
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
    if (err) {
        stop();
    }
    do_read();
}

void talk_to_server::on_move(const std::string& str) {
    char ch = str.front();
    if (ch == EnumToChar(Request::X_PLUS_Y_PLUS)) {
        for (auto& x : cubes) {
            x->Move(Request::X_PLUS_Y_PLUS);
        }
    } else if (ch == EnumToChar(Request::X_PLUS_Y_MINUS)) {
        for (auto& x : cubes) {
            x->Move(Request::X_PLUS_Y_MINUS);
        }
    } else if (ch == EnumToChar(Request::X_MINUS_Y_PLUS)) {
        for (auto& x : cubes) {
            x->Move(Request::X_MINUS_Y_PLUS);
        }
    } else if (ch == EnumToChar(Request::X_MINUS_Y_MINUS)) {
        for (auto& x : cubes) {
            x->Move(Request::X_MINUS_Y_MINUS);
        }
    } else if (ch == EnumToChar(Request::X_PLUS)) {
        for (auto& x : cubes) {
            x->Move(Request::X_PLUS);
        }
    } else if (ch == EnumToChar(Request::X_MINUS)) {
        for (auto& x : cubes) {
            x->Move(Request::X_MINUS);
        }
    } else if (ch == EnumToChar(Request::Y_PLUS)) {
        for (auto& x : cubes) {
            x->Move(Request::Y_PLUS);
        }
    } else if (ch == EnumToChar(Request::Y_MINUS)) {
        for (auto& x : cubes) {
            x->Move(Request::Y_MINUS);
        }
    }
    do_read();
}

void talk_to_server::on_read(const error_code& err, size_t bytes) {
    if (err) {
        stop();
    }
    if (!started()) {
        return;
    }
    std::string msg(read_buffer, bytes);
    std::cout << "Recived: " << msg << " " << msg.size() << std::endl;
    if (msg.front() == EnumToChar(Request::LOGIN_OK)) {
        on_login();
    } else if (msg.front() == EnumToChar(Request::NEXT_LEVEL)) {
        NextLevel();
        do_read();
    } else if (msg.front() == EnumToChar(Request::RESTART)) {
        Restart();
        do_read();
    } else {
        on_move(msg);
    }
}

void talk_to_server::on_login() {
    std::cout << "Login OK" << std::endl << "My name is " << s_username << std::endl;
    do_read();
}

void talk_to_server::on_write(const error_code& err, size_t bytes) {
    //do_read();
}
void talk_to_server::do_read() {
    std::cout << "Reading..." << std::endl;
    async_read(m_sock, buffer(read_buffer), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
}

void talk_to_server::do_write(const std::string& msg) {
    if (!b_started) {
        return;
    }
    std::copy(msg.begin(), msg.end(), write_buffer);
    m_sock.async_write_some(buffer(write_buffer, msg.size()), MEM_FN2(on_write, _1, _2));
}

void talk_to_server::do_write(const Request request) {
    if (!b_started) {
        return;
    }
    write_buffer[0] = EnumToChar(request);
    write_buffer[1] = '\n';
    m_sock.async_write_some(buffer(write_buffer, 2), MEM_FN2(on_write, _1, _2));
}

void talk_to_server::do_write(const Request request, const std::string& msg) {
    if (!b_started) {
        return;
    }
    write_buffer[0] = EnumToChar(request);
    std::copy(msg.begin(), msg.end(), write_buffer + 1);
    write_buffer[msg.size()] = '\n';
    m_sock.async_write_some(buffer(write_buffer, msg.size() + 2), MEM_FN2(on_write, _1, _2));
}

size_t talk_to_server::read_complete(const boost::system::error_code& err, size_t bytes) {
    if (err) {
        return 0;
    }
    //std::cout << "Read_buffer: " << read_buffer << std::endl;
    bool find = std::find(read_buffer, read_buffer + bytes, '\n') < read_buffer + bytes;
    return find ? 0 : 1;
}









