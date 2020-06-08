//
// Created by Ameen Maali on 6/1/20.
//

#ifndef URLDEDUPE_FLAGS_HPP
#define URLDEDUPE_FLAGS_HPP

#include <string>

struct Flag
{
    std::string short_name;
    std::string long_name;
    std::string usage;
    bool required;
    bool is_switch;
};

struct Option
{
    Flag flag;
    std::string value;
};

std::vector<Option> parse_flags(int argc, char **argv);
void print_version(const std::string &version);
void print_usage();

#endif // URLDEDUPE_FLAGS_HPP
