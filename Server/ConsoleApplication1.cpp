//g++ X.cpp -pthread -lboost_thread

#include "Server_request.h"

/*******************************************/
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
/*****************************************/

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/core/noncopyable.hpp>

#include <iostream>
#include <vector>

sql::mysql::MySQL_Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::ResultSet* result;


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

    bool started() const { return b_started; }
    ip::tcp::socket& sock() { return m_sock; }
    std::string username() const { return s_username; }
    void set_clients_changed() { b_clients_changed = true; }

private:
    void on_read(const error_code& err, size_t bytes) {
        if (err) {
            stop();
        }
        if (!b_started) {
            return;
        }
        std::string msg(read_buffer, bytes);
        std::cout << " send: " << msg;
        if (msg.front() == EnumToChar(Request::LOGIN)) {
            on_login(msg);
        } else if (msg.front() == EnumToChar(Request::SERVER_QUIT)) {
            std::cout << "SERVER UIT" << std::endl;
            exit(1);
        } else if (msg.front() == EnumToChar(Request::SERVER_QUIT)) {
            std::cout << "quit: " << std::endl;
            stop();
        } else {
            do_write_other(msg);
            do_read();
        }
    }

    void on_move(const std::string& msg) {
        for (auto iter = clients.begin(); iter != clients.end(); iter++) {
            std::cout << (*iter)->s_username << " " << msg << std::endl;
            if (iter + 1 == clients.end()) {
                (*iter)->m_sock.async_write_some(buffer(msg), MEM_FN2(on_write, _1, _2));
            }
            else {
                write((*iter)->m_sock, buffer(msg));
            }
        }
    }
    void on_login(const std::string& msg) {
        std::istringstream in(msg);
        char ch;
        in >> ch >> s_username;
        pstmt = con->prepareStatement("SELECT id FROM table_main WHERE login = ?");
        pstmt->setString(1, s_username.c_str());
        result = pstmt->executeQuery();
        if (result->next()) {
            std::cout << result->getInt(1) << std::endl;
            std::cout << "NONE" << std::endl;
        } else {
            pstmt = con->prepareStatement("INSERT INTO table_main(login, password) VALUES(?, 'password')");
            pstmt->setString(1, s_username);
            pstmt->execute();
            std::cout << s_username << " connected" << std::endl;
            do_write(EnumToString(Request::LOGIN_OK) + '\n');
        }
        
        do_read();
    }
    void on_write(const error_code& err, size_t bytes) {
       // do_read();
    }
    void do_read() {
        async_read(m_sock, buffer(read_buffer), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
    }
    void do_read_enum() {
        m_sock.async_read_some(buffer(read_buffer), MEM_FN2(on_read, _1, _2));
    }
    void do_write(const std::string& msg) {
        if (!started()) {
            return;
        }
    std::cout << msg.size() << " " << msg;
        std::copy(msg.begin(), msg.end(), write_buffer);
        m_sock.async_write_some(buffer(write_buffer, msg.size()), MEM_FN2(on_write, _1, _2));
    }
    void do_write_enum(Request x) {
        if (!started()) {
            return;
        }
        m_sock.async_write_some(buffer(EnumToString(x)), MEM_FN2(on_write, _1, _2));
    }
    void do_write_other(const std::string& msg) {
        if (!started()) {
            return;
        }
        // std::string str = "AAA\n";
        std::copy(msg.begin(), msg.end(), write_buffer);
        //  m_sock.async_write_some(buffer(write_buffer, msg.size()), MEM_FN2(on_write,_1,_2));

        for (const auto& iter : clients) {
            if (iter.get() != this) {
                iter->m_sock.async_write_some(buffer(write_buffer, msg.size()), MEM_FN2(on_write, _1, _2));
            }
        }

    }
    size_t read_complete(const boost::system::error_code& err, size_t bytes) {
        if (err) {
            return 0;
        }
        bool find = std::find(read_buffer, read_buffer + bytes, '\n') < read_buffer + bytes;
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

void ServerRun() {
    while (true) {
        std::cout << "Enter your request: ";
        std::string str;
        std::getline(std::cin, str);
        if (str == "exit") {
            exit(1);
        }
    }
}

int main() {


    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
        con->setSchema("my_database");
        stmt = con->createStatement();
        /******************************************/
        /*
        int i = 2;
        pstmt = con->prepareStatement("SELECT name FROM inventory WHERE id = ?");
        pstmt->setInt(1, i);
        result = pstmt->executeQuery();
       // result = stmt->executeQuery("SELECT id FROM inventory WHERE id = ?");

        if (result->next())
            std::cout << "Reading from table = " << result->getString(1);
        else {
            std::cout << "NONE" << std::endl;
        }
        std::cout << std::endl;
        */
    } catch (sql::SQLException e) {
        std::cout << "Error! " << e.what() << std::endl;
    }

    std::cout << "Server" << std::endl;
    // auto f = std::async([&]{ServerRun();});

    talk_to_client::ptr client = talk_to_client::new_talk_to_client();
    acc.async_accept(client->sock(), boost::bind(handle_accept, client, _1));
    service.run();

    std::cout << "END" << std::endl;
    return 0;
}






/*
#include <iostream>
#include <stdlib.h>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

int main()
{

    //  sql::Driver* driver;
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;
    sql::PreparedStatement* pstmt;
    sql::ResultSet* result;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
        
       // driver = get_driver_instance();
        //con = driver->connect("tcp://127.0.0.1:3306", "root", "root");

        con->setSchema("my_database");

        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS inventory");
        cout << "Finished dropping table (if existed)" << endl;
        stmt->execute("CREATE TABLE inventory (id serial PRIMARY KEY, name VARCHAR(50), quantity INTEGER);");
        cout << "Finished creating table" << endl;
        //delete stmt;

        pstmt = con->prepareStatement("INSERT INTO inventory(name, quantity) VALUES(?,?)");
        pstmt->setString(1, "banana");
        pstmt->setInt(2, 150);
        pstmt->execute();
        cout << "One row inserted." << endl;

        pstmt->setString(1, "orange");
        pstmt->setInt(2, 154);
        pstmt->execute();
        cout << "One row inserted." << endl;

        pstmt->setString(1, "apple");
        pstmt->setInt(2, 100);
        pstmt->execute();
        cout << "One row inserted." << endl;
       
        result = stmt->executeQuery("SELECT name FROM inventory WHERE id = 2 AND quantity = 150;");
        if (result->next())
        std::cout << "Reading from table = " << result->getString(1);
        else {
            std::cout << "NONE" << std::endl;
        }
        std::cout << std::endl;

 
        while (result->next())
            // printf("Reading from table=(%d, %s, %d)\n", result->getInt(1), result->getString(2).c_str(), result->getInt(3));
            std::cout << "Reading from table = " << result->getInt(1);// << " " << result->getString(2) << " " << result->getInt(3) << std::endl;
        
       
    }
    catch (sql::SQLException e) {

        std::cout << "Error! " << e.what() << std::endl;
    }


//    delete stmt;
  //  delete con;
}

*/