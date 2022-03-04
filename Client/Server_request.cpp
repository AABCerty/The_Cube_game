#include "Server_request.h"

#include <string>
#include <iostream>

char EnumToChar(const Request request) {
    switch (request) {
        case Request::QUIT :            return 'q';
        case Request::X_PLUS_Y_PLUS :   return 'w';
        case Request::X_PLUS_Y_MINUS :  return 'e';
        case Request::X_MINUS_Y_PLUS :  return 'r';
        case Request::X_MINUS_Y_MINUS : return 't';
        case Request::X_PLUS :          return 'y';
        case Request::X_MINUS :         return 'u';
        case Request::Y_PLUS :          return 'i';
        case Request::Y_MINUS :         return 'o';
        case Request::LOGIN :           return 'p';
        case Request::LOGIN_OK :        return 'a';
        case Request::SERVER_QUIT :     return 's';
        case Request::NEXT_LEVEL :      return 'd';
        case Request::RESTART :         return 'f';
    }
    return 'q';
}



std::string EnumToString(const Request request) {
    std::string str = "A";
    str[0] = EnumToChar(request);
    return str;
}



std::pair<int, int> EnumToMove(const Request request) {
    switch (request) {
        case Request::X_PLUS_Y_PLUS :   return {1, 1};
        case Request::X_PLUS_Y_MINUS :  return {1, -1};
        case Request::X_MINUS_Y_PLUS :  return {-1, 1};
        case Request::X_MINUS_Y_MINUS : return {-1, -1};
        case Request::X_PLUS :          return {1, 0};
        case Request::X_MINUS :         return {-1, 0};
        case Request::Y_PLUS :          return {0, 1};
        case Request::Y_MINUS :         return {0, -1};
        case Request::QUIT :
        case Request::LOGIN :
        case Request::LOGIN_OK :
        case Request::SERVER_QUIT :
        case Request::NEXT_LEVEL :
        case Request::RESTART :         ;
    }
    return {0, 0};
}

