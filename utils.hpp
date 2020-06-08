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

inline char hex_digit(char c)
{
    if ('0' <= c and c <= '9')
        return (c - '0');
    if ('a' <= c and c <= 'f')
        return (c - 'a' + 10);
    if ('A' <= c and c <= 'F')
        return (c - 'A' + 10);
    return -1; // invalid hex digit; maybe throw instead?
}

// @brief Clone a string view referring to a string
// @param str1 Some string
// @param sv1 A string_view into str1
// @param str2 A copy of str1 (or at least a string of the same length)
// @return A string_view referring to the same substring of str2 as sv1 of str1
inline std::string_view clone_string_view(const std::string &str1, const std::string_view &sv1, const std::string &str2)
{
    if (sv1.length() == 0)
        return std::string_view(); // Empty string_view
    auto start = sv1.data() - str1.data();

    return std::string_view(str2).substr(start, sv1.length());
}

#endif // URLDEDUPE_UTILS_HPP
