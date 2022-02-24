#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#undef main
//195.161.68.193
void connect_handler(const boost::system::error_code & ec);

using namespace boost::asio;

io_service service;
ip::tcp::socket sock(service);
void connect_handler(const boost::system::error_code & ec);
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

SDL_Window *win = nullptr;

size_t read_complete(char * buf, const boost::system::error_code & err, size_t bytes)
{
	if ( err) return 0;
	bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
	// we read one-by-one until we get to enter, no buffering
	return found ? 0 : 1;
}

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    win = SDL_CreateWindow("Примитивы", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Event e;
    std::string str;
    bool run = true;

    std::cout << "Client" << std::endl;
    //ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
    ip::tcp::endpoint ep(ip::address::from_string("195.161.68.193"), 2001);
    sock.connect(ep);


    char buff[100];
    char buff_read[100];
    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);
    sock.write_some(buffer(name.c_str(), name.size()));
    sock.read_some(buffer(buff));
    std::cout << buff << std::endl;

    while (run) {
        if (sock.available()) {
            sock.read_some(buffer(buff));
            std::cout << buff << std::endl;
        }
        std::string str_temp = buff;
        if (str_temp == "wait") {
            sock.read_some(buffer(buff));
            read(sock, buffer(buff), boost::bind(read_complete,buff,_1,_2));
            std::cout << buff << std::endl;
        }

        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    std::cout << "UP" << std::endl;
                    sock.write_some(buffer("wait", 4));
                    std::getline(std::cin, str);
                    str += '\n';
                    sock.write_some(buffer(str));
                    break;
                case SDLK_DOWN:
                    std::cout << "DOWN" << std::endl;
                    sock.write_some(buffer("quit", 4));
                    run = false;
                    break;
                case SDLK_LEFT:
                    std::cout << "Left" << std::endl;
                    sock.write_some(buffer("server_quit", 11));
                    run = false;
                    break;
                } // switch
            }   // if
        }   // while Event
    } // while run




    std::cout << "END" << std::endl;

    return 0;
}

void connect_handler(const boost::system::error_code & ec) {
    std::cout << "AA" << std::endl;
    write(sock, buffer("Hello", 5));
    sleep (5);
    char data[512];
    size_t len = sock.read_some(buffer(data));
    if ( len > 0) {
        std::cout << "data: " << data << std::endl;
        //write(sock, buffer("ok", 2));
    }
    sleep (2);

}
