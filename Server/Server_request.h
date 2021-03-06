#pragma once


#include <string>

enum class Request {
    QUIT,                   // 0    q
    X_PLUS_Y_PLUS,          // 1    w
    X_PLUS_Y_MINUS,         // 2    e
    X_MINUS_Y_PLUS,         // 3    r
    X_MINUS_Y_MINUS,        // 4    t
    X_PLUS,                 // 5    y
    X_MINUS,                // 6    u
    Y_PLUS,                 // 7    i
    Y_MINUS,                // 8    o
    LOGIN,                  // 9    p
    LOGIN_OK,               // 10   a
    SERVER_QUIT,            // 11   s
    NEXT_LEVEL,             // 12   d
    RESTART,                // 13   f
    TEXT,                   // 14   g
};


char EnumToChar(const Request request);

std::string EnumToString(const Request request);

std::pair<int, int> EnumToMove(const Request request);









#pragma once
