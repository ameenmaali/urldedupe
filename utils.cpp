//
// Created by Ameen Maali on 6/1/20.
//

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "utils.hpp"

bool load_urls_from_file(std::vector<std::string> &urls, const std::string &filename)
{
    std::ifstream file (filename);
    if (!file.is_open()) {
        std::cout << "Unable to open file " << filename << std::endl;
        return false;
    }

    std::string url;
    while (file.good()) {
        getline(file, url);
        if (!url.empty())
        {
            urls.push_back(url);
        }
    }
    file.close();

    return true;
}