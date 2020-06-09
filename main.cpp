#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Url.hpp"
#include "flags.hpp"
#include "utils.hpp"

const std::string VERSION {"1.0.4"};

int main(int argc, char **argv)
{
    CliOptions cli_options {};
    // Parse flags if provided
    std::vector<Option> options;
    if (argc > 1)
    {
        options = parse_flags(argc, argv);
    }

    std::vector<Url> urls {};
    std::string filename {};

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

        parse_cli_options(option, cli_options);

    }

    if (cli_options.filename.length() > 0)
        load_urls_from_file(urls, cli_options.filename, cli_options.regex_mode);
    else
        read_urls_from_stream(urls, std::cin, cli_options.regex_mode);

    std::unordered_map<std::string, bool> deduped_url_keys;
    for (auto &parsed_url: urls)
    {
        // Move on to the next if -qs is enabled and URL has no query strings
        if (cli_options.query_strings_only)
        {
            if (parsed_url.get_query_strings().empty())
                continue;
        }

        if (cli_options.no_extensions_only)
        {
            if (parsed_url.has_extension())
                continue;
        }

        std::string url_key {parsed_url.get_url_key(cli_options.similar_mode)};
        if (deduped_url_keys.find(url_key) != deduped_url_keys.end())
            continue;

        deduped_url_keys.insert(std::make_pair(url_key, true));

        // If it has made it to this point, it's a non-duplicate URL. Print it
        std::cout << parsed_url.get_url_string() << std::endl;
    }

    return 0;
}
