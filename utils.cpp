//
// Created by Ameen Maali on 6/1/20.
//

#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Url.hpp"

bool load_urls_from_file(std::vector<Url> &urls, const std::string &filename, bool regex_mode)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cout << "Unable to open file " << filename << std::endl;
        return false;
    }
    else
    {
        read_urls_from_stream(urls, file, regex_mode);
        return true;
    }
}

void read_urls_from_stream(std::vector<Url> &urls, std::istream &is, bool regex_mode)
{
    std::string s;
    while (getline(is, s))
    {
        urls.emplace_back(s, regex_mode);
    }
}

bool is_number(const std::string &str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}
