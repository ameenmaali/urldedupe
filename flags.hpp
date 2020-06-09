//
// Created by Ameen Maali on 6/1/20.
//

#ifndef URLDEDUPE_FLAGS_HPP
#define URLDEDUPE_FLAGS_HPP

#include <string>

#include "utils.hpp"

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

struct CliOptions
{
    bool regex_mode {false};
    bool similar_mode {false};
    bool query_strings_only {false};
    bool no_extensions_only {false};
    std::string filename {};
};

std::vector<Option> parse_flags(int argc, char **argv);
void print_version(const std::string &version);
void print_usage();
void parse_cli_options(const Option &option, CliOptions &cli_options);

#endif // URLDEDUPE_FLAGS_HPP
