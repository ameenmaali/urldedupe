//
// Created by Ameen Maali on 6/1/20.
//

#ifndef URLDEDUPE_UTILS_HPP
#define URLDEDUPE_UTILS_HPP

#include <string>
#include <vector>

class Url;

bool load_urls_from_file(std::vector<Url> &urls, const std::string &filename, bool regex_mode);

void read_urls_from_stream(std::vector<Url> &urls, std::istream &is, bool regex_mode);

bool is_number(const std::string &str);

inline char hex_digit(char c) {
    if('0' <= c and c <= '9') return (c - '0');
    if('a' <= c and c <= 'f') return (c - 'a' + 10);
    if('A' <= c and c <= 'F') return (c - 'A' + 10);
    return -1;  // invalid hex digit; maybe throw instead?
}

#endif //URLDEDUPE_UTILS_HPP
