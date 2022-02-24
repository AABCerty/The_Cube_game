//g++ X.cpp -pthread -lboost_thread

#include "Server_request.h"


#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/core/noncopyable.hpp>

#include <iostream>
#include <vector>
using namespace boost::asio;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

io_service service;
ip::tcp::acceptor acc(service, ip::tcp::endpoint(ip::tcp::v4(), 2001));

class talk_to_client;
typedef boost::shared_ptr<talk_to_client> client_ptr;
std::vector<client_ptr> clients;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)

class talk_to_client : public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable
{
    typedef talk_to_client self_type;
    talk_to_client() : m_sock(service), b_started(false), m_timer(service), b_clients_changed(false) {
    }
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_client> ptr;

    void start() {
        b_started = true;
        clients.push_back(shared_from_this());
        m_last_ping = boost::posix_time::microsec_clock::local_time();
        do_read();
    }
    static ptr new_talk_to_client() {
        ptr new_talk_to_client(new talk_to_client);
        return new_talk_to_client;
    }
    void stop() {
        if (!b_started) {
            return;
        }
        b_started = false;
        m_sock.close();
        ptr self = shared_from_this();
        std::vector<client_ptr>::iterator it = std::find(clients.begin(), clients.end(), self);
        clients.erase(it);
    }
    bool started() const {
        return b_started;
    }
    ip::tcp::socket& sock() {
        return m_sock;
    }
    std::string username() const {
        return s_username;
    }
    void set_clients_changed() {
        b_clients_changed = true;
    }
private:
    void on_read(const error_code& err, size_t bytes) {
        if (err) {
            stop();
        }
        if (!b_started) {
            return;
        }
        std::string msg(read_buffer, bytes);
        std::cout << "on_read: " << msg << ", " << msg.front() << std::endl;
        if (msg.front() == EnumToChar(LOGIN)) {
            on_login(msg);
        } else {
             std::cout << "read_else " << std::endl;
            // NEED to send all message?
            for (const auto& x : msg) {
                if (x == EnumToChar(SERVER_QUIT)) {
                    std::cout << "SERVER UIT" << std::endl;
                    exit(1);
                } else if (x == EnumToChar(QUIT)) {
                    std::cout << "quit: " << std::endl;
                    stop();
                } else {
                    do_write_char(x);
                }
            }
            /*
            if (msg.back() == static_cast<char>(SERVER_QUIT)) {
                exit(1);
            } else if (msg.back() == static_cast<char>(QUIT)) {
                stop();
            } else {
            }
            */
        }
        std::cout << "AA" << std::endl;
    }
    void on_move(const std::string& msg) {
          for (auto iter = clients.begin(); iter != clients.end(); iter++) {
            std::cout << (*iter)->s_username << " " << msg << std::endl;
            if (iter+1 == clients.end()){
                (*iter)->m_sock.async_write_some(buffer(msg), MEM_FN2(on_write,_1,_2));
            } else {
                write((*iter)->m_sock, buffer(msg));
            }
        }
    }
    void on_login(const std::string& msg) {
        std::cout << "on_login" << std::endl;
        std::istringstream in(msg);
        char ch;
        in >> ch >> s_username;
        std::cout << s_username << " connected" << std::endl;
        do_write_enum(LOGIN_OK);
    }
    void on_write(const error_code& err, size_t bytes) {
        std::cout << "on_write" << std::endl;
        do_read_enum();
    }
    void do_read() {
        std::cout << "do_read" << std::endl;
        async_read(m_sock, buffer(read_buffer), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
    }
    void do_read_enum() {
        std::cout << "do_read_enum" << std::endl;
        m_sock.async_read_some(buffer(read_buffer), MEM_FN2(on_read, _1, _2));
    }
    void do_write(const std::string& msg) {
        if (!started()) {
            return;
        }
        std::copy(msg.begin(), msg.end(), write_buffer);
        m_sock.async_write_some(buffer(write_buffer, msg.size()), MEM_FN2(on_write,_1,_2));
    }
    void do_write_enum(int x) {
        if (!started()) {
            return;
        }
        m_sock.async_write_some(buffer(EnumToString(x)), MEM_FN2(on_write,_1,_2));
    }
    void do_write_char(char ch) {
        if (!started()) {
            return;
        }
        char temp_ch[2];
        temp_ch[0] = ch;
        temp_ch[1] = '\0';
        std::cout << ch << ", " << std::to_string(ch) << ", " << temp_ch << std::endl;
       // m_sock.async_write_some(buffer(temp_ch), MEM_FN2(on_write,_1,_2));
        for (const auto& iter : clients) {
            if (iter.get() != this) {
                iter->m_sock.async_write_some(buffer(temp_ch), MEM_FN2(on_write,_1,_2));
            }
        }
    }
    size_t read_complete(const boost::system::error_code & err, size_t bytes) {
        if (err) {
            return 0;
        }
        std::cout << read_buffer << std::endl;
        bool find = std::find(read_buffer, read_buffer+bytes, '\n') < read_buffer + bytes;
        return find ? 0 : 1;
    }

private:
	ip::tcp::socket m_sock;
	enum { max_msg = 1024 };
	char read_buffer[max_msg];
	char write_buffer[max_msg];
	bool b_started;
	std::string s_username;
	deadline_timer m_timer;
	boost::posix_time::ptime m_last_ping;
	bool b_clients_changed;
};

void handle_accept(talk_to_client::ptr client, const boost::system::error_code& err) {
    client->start();
    talk_to_client::ptr new_client = talk_to_client::new_talk_to_client();
    acc.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
}

int main() {
    std::cout << "enum: " << LOGIN << std::endl;
    std::cout << "enum_to_char: " << static_cast<char>(LOGIN) << std::endl;
    std::cout << "enum_to_int: " << static_cast<int>(LOGIN) << std::endl;
//    std::cout << "enum_to_string: " << static_cast<std::string>(LOGIN) << std::endl;
    std::cout << "enum_to_char_to_string: " << std::to_string(static_cast<char>(LOGIN)) << std::endl;
    char ch = '9';
    std::cout << "char: " << ch << std::endl;
    std::cout << "char_to_int: " << static_cast<int>(ch) << std::endl;
    int num = 9;
    std::cout << "int_to_char: " << static_cast<char>(9) << std::endl;
    ch = num;
    std::cout << "char = num: " << ch << std::endl;

    std::string str = std::to_string(ch);
    std::cout << str << ", " << std::to_string(static_cast<char>(LOGIN)) << std::endl;


    talk_to_client::ptr client = talk_to_client::new_talk_to_client();
    acc.async_accept(client->sock(), boost::bind(handle_accept, client, _1));
    service.run();


    std::cout << "END" << std::endl;
    return 0;
}

