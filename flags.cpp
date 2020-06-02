//
// Created by Ameen Maali on 6/1/20.
//

#include <iostream>
#include <vector>
#include "flags.hpp"

std::vector<Flag> get_flags()
{
    std::vector<Flag> flags {};

    Flag help {.short_name="-h", .long_name="--help", .usage="Usage/help info for urldedupe", .required=false, .is_switch=true};
    Flag urls {.short_name="-u", .long_name="--urls", .usage="Filename containing urls (use this if you don't pipe urls via stdin)", .required=false, .is_switch=false};
    Flag version {.short_name="-V", .long_name="--version", .usage="Get current version for urldedupe", .required=false, .is_switch=true};
    Flag regex {.short_name="-r", .long_name="--regex-parse", .usage="This is significantly slower than normal parsing, but may be more thorough or accurate", .required=false, .is_switch=true};

    flags.push_back(help);
    flags.push_back(urls);
    flags.push_back(version);
    flags.push_back(regex);

    return flags;
}

std::vector<Option> parse_flags(int argc, char **argv)
{
    std::vector<Flag> flags {get_flags()};
    std::vector<Option> options {};

    for (int i {0}; i < argc; i++) {
        for (const auto &flag: flags)
        {
            if (argv[i] == flag.short_name || argv[i] == flag.long_name)
            {
                if (i+1 >= argc && !flag.is_switch)
                {
                    std::cerr << "Flag " << flag.long_name << " exists, but requires a value" << std::endl;
                    continue;
                }

                Option opt;
                if (!flag.is_switch) {
                    opt = {.flag=flag, .value=argv[i+1]};
                } else {
                    opt = {.flag=flag, .value=""};
                }
                options.push_back(opt);
            }
        }
    }
    return options;
}

void print_version(const std::string &version)
{
    std::cout << "Current urldupe version is " << version << std::endl;
}

void print_usage()
{
    std::vector<Flag> flags {get_flags()};

    for (Flag &flag: flags)
    {
        std::cout << "(" << flag.short_name << "|" << flag.long_name << ") - " << flag.usage << std::endl;
    }
}
