#include "Server_request.h"

#include <string>
#include <iostream>
#include <map>

char EnumToChar(int x) {
    // Switch
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
    } else if (x == 12) {
        return 'd';
    } else if (x == 13) {
        return 'f';
    }
    return 'a';
}

std::string EnumToString(int x) {
    std::string str = "A";
    str[0] = EnumToChar(x);
    return str;
}

std::pair<int, int> EnumToMove(int x) {
    // Switch
    if (x == X_PLUS_Y_PLUS) {
        return {1,1};
    } else if (x == X_PLUS_Y_MINUS) {
        return {1, -1};
    } else if (x == X_MINUS_Y_PLUS) {
        return {-1, 1};
    } else if (x == X_MINUS_Y_MINUS) {
        return {-1, -1};
    } else if (x == X_PLUS) {
        return {1, 0};
    } else if (x == X_MINUS) {
        return {-1, 0};
    } else if (x == Y_PLUS) {
        return {0, 1};
    } else if (x == Y_MINUS) {
        return {0, -1};
    }
    return {0, 0};
}
