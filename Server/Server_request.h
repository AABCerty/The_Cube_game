#pragma once

#include <string>

enum {
    QUIT,                   // q
    X_PLUS_Y_PLUS,          // w
    X_PLUS_Y_MINUS,         // e
    X_MINUS_Y_PLUS,         // r
    X_MINUS_Y_MINUS,        // t
    X_PLUS,                 // y
    X_MINUS,                // u
    Y_PLUS,                 // i
    Y_MINUS,                // o
    LOGIN,                  // p
    LOGIN_OK,               // a
    SERVER_QUIT,            // s
};


char EnumToChar(int x) {
    if (x == 0) {
        return 'q';
    } else if (x == 1) {
        return 'w';
    } else if (x == 2) {
        return 'e';
    } else if (x == 3) {
        return 'r';
    } else if (x == 4) {
        return 't';
    } else if (x == 5) {
        return 'y';
    } else if (x == 6) {
        return 'u';
    } else if (x == 7) {
        return 'i';
    } else if (x == 8) {
        return 'o';
    } else if (x == 9) {
        return 'p';
    } else if (x == 10) {
        return 'a';
    } else if (x == 11) {
        return 's';
    }
    return 'n';
}

std::string EnumToString(int x) {
    std::string str = "A";
    str[0] = EnumToChar(x);
    return str;
}
