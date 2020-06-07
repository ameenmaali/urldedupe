#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "flags.hpp"
#include "utils.hpp"
#include "Url.hpp"

const std::string VERSION {"1.0.2"};

int main(int argc, char **argv) {
    // Parse flags if provided
    std::vector<Option> options;
    if (argc > 1)
    {
        options = parse_flags(argc, argv);
    }

    std::vector<Url> urls {};
    std::string filename {};

    bool regex_mode, silent_mode {false};
    for (const Option &option: options)
    {
        if (option.flag.short_name == "-h")
        {
            print_usage();
            return 0;
        }

        if (option.flag.short_name == "-V")
        {
            print_version(VERSION);
            return 0;
        }

        if (option.flag.short_name == "-u")
            filename = option.value;

        if (option.flag.short_name == "-r")
            regex_mode = true;

        if (option.flag.short_name == "-s")
            silent_mode = true;
    }

    if (filename.length() > 0) {
        load_urls_from_file(urls, filename, regex_mode);
    }
    else {
        read_urls_from_stream(urls, std::cin, regex_mode);
    }

    std::unordered_map<std::string, bool> deduped_url_keys;
    for (auto &parsed_url: urls)
    {
        std::string url_key {parsed_url.get_url_key(silent_mode)};
        if (deduped_url_keys.find(url_key) != deduped_url_keys.end())
            continue;

        deduped_url_keys.insert(std::make_pair(url_key, true));

        // If it has made it to this point, it's a non-duplicate URL. Print it
        std::cout << parsed_url.get_url_string() << std::endl;
    }

    return 0;
}
