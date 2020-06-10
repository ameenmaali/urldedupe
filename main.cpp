#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
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

    std::unique_ptr<std::ifstream> file_ptr; // Lifetime management
    auto &input_stream = [&]() -> std::istream & {
        if (cli_options.filename.length() > 0)
        {
            file_ptr = std::make_unique<std::ifstream>(cli_options.filename);
            if (!(*file_ptr))
            {
                std::cout << "Unable to open file " << cli_options.filename << std::endl;
                std::exit(-1);
            }
            else
            {
                return *file_ptr;
            }
        }
        else
        {
            return std::cin;
        }
    }();

    std::unordered_set<std::string> deduped_url_keys;
    for (auto it = std::istream_iterator<std::string>(input_stream); it != std::istream_iterator<std::string>(); it++)
    {
        auto parsed_url = Url(*it, cli_options.regex_mode);
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
        auto [_, inserted] = deduped_url_keys.insert(url_key);

        if (inserted)
        {
            std::cout << parsed_url.get_url_string() << "\n";
        }
    }

    return 0;
}
