//
// Created by Ameen Maali on 6/1/20.
//

#ifndef URLDEDUPE_UTILS_HPP
#define URLDEDUPE_UTILS_HPP

#include <string>
#include <vector>

bool load_urls_from_file(std::vector<std::string> &urls, const std::string &filename);

inline char hex_digit(char c) {
    if('0' <= c and c <= '9') return (c - '0');
    if('a' <= c and c <= 'f') return (c - 'a' + 10);
    if('A' <= c and c <= 'F') return (c - 'A' + 10);
    return -1;  // invalid hex digit; maybe throw instead?
}

#endif //URLDEDUPE_UTILS_HPP
