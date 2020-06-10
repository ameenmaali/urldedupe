//
// Created by Ameen Maali on 6/1/20.
//

#include <filesystem>
#include <regex>

#include "Url.hpp"
#include "utils.hpp"

Url::Url(const std::string &url, bool regex_mode) : url_string(url)
{
    if (regex_mode)
    {
        this->regex_parse();
    }
    else
    {
        this->parse();
    }
}

Url::Url(const Url &other) : url_string(other.url_string)
{
    scheme = clone_string_view(other.url_string, other.scheme, url_string);
    hostname = clone_string_view(other.url_string, other.hostname, url_string);
    path = clone_string_view(other.url_string, other.path, url_string);
    query_strings = clone_string_view(other.url_string, other.query_strings, url_string);
    fragment = clone_string_view(other.url_string, other.fragment, url_string);
}

std::string_view Url::get_scheme() const
{
    return scheme;
}

std::string_view Url::get_hostname() const
{
    return hostname;
}

std::string_view Url::get_path() const
{
    return path;
}

std::string_view Url::get_query_strings() const
{
    return query_strings;
}

std::string_view Url::get_fragment() const
{
    return fragment;
}

const std::string &Url::get_url_string() const
{
    return url_string;
}

bool Url::is_encoded(const std::string &u)
{
    return u.find('%') != std::string::npos;
}

std::string Url::decode(const std::string &str)
{
    std::string ret;
    auto len = str.length();

    for (std::size_t i = 0; i < len; i++)
    {
        if (str[i] != '%')
        {
            if (str[i] == '+')
            {
                ret += ' ';
            }
            else
            {
                ret += str[i];
            }
        }
        else
        {
            // If str[i+2] does not exist, this will crash.
            // This also means the URL is invalid, so...
            ret += hex_digit(str[i + 1]) * 16 + hex_digit(str[i + 2]);
            i = i + 2;
        }
    }
    return ret;
}

std::string Url::encode(const std::string &str)
{
    std::string encoded_str {};
    char bufHex[10];
    const int len = str.length();

    for (int i = 0; i < len; i++)
    {
        auto c = str[i];

        if (c == ' ')
        {
            encoded_str += '+';
        }
        else
        {
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            {
                encoded_str += c;
            }
            else
            {
                sprintf(bufHex, "%X", c);

                if (static_cast<int>(c) < 16)
                {
                    encoded_str += "%0";
                }
                else
                {
                    encoded_str += "%";
                }
                encoded_str += bufHex;
            }
        }
    }
    return encoded_str;
}

void Url::regex_parse()
{
    auto make_sv = [&](const std::ssub_match &m) -> std::string_view {
        if (not m.matched)
            return {};
        auto start = m.first - std::begin(url_string);
        return std::string_view(url_string.data() + start, m.length());
    };
    std::smatch match;
    if (std::regex_match(url_string, match, URL_REGEX))
    {
        scheme = make_sv(match[2]);
        hostname = make_sv(match[4]);
        path = make_sv(match[5]);
        query_strings = make_sv(match[7]);
        fragment = make_sv(match[9]);
    }
}

bool Url::parse()
{
    std::string_view url_view {url_string};

    auto current {url_view.find("://")};
    if (current != std::string::npos)
    {
        scheme = url_view.substr(0, current + 3);
        url_view = url_view.substr(current + 3);
    }

    current = url_view.find('#');
    if (current != std::string::npos)
    {
        fragment = url_view.substr(current + 1);
        url_view = url_view.substr(0, current);
    }

    current = url_view.find('?');
    if (current != std::string::npos)
    {
        query_strings = url_view.substr(current + 1);
        url_view = url_view.substr(0, current);
    }

    current = url_view.find('/');
    if (current != std::string::npos)
    {
        path = url_view.substr(current);
    }
    hostname = url_view.substr(0, current);

    return true;
}

std::string Url::get_url_key(bool similar_mode)
{
    std::string url_key {};
    url_key += this->hostname;
    if (similar_mode)
    {
        url_key += this->get_path_components();
    }
    else
    {
        url_key += this->path;
    }

    std::string qs {get_query_strings()};
    if (qs.empty())
        return url_key;

    std::string token {};
    size_t current;
    std::vector<std::string> qs_vals {};
    qs += "&";
    while ((current = qs.find('&')) != std::string::npos)
    {
        token = qs.substr(0, current);
        qs.erase(0, current + 1);

        std::string qs_key {token.substr(0, token.find('='))};
        qs_vals.push_back(qs_key);
    }

    url_key += "?";
    for (const auto &x: qs_vals)
        url_key += x + "&";

    return url_key;
}

std::string Url::get_path_components() const
{
    std::string path_components {};
    if (this->path.empty())
        return path_components;

    std::string url_path {this->path};
    std::string token {};
    size_t current;

    // Add trailing slash to get all path components (including last)
    url_path += "/";
    while ((current = url_path.find('/')) != std::string::npos)
    {
        token = url_path.substr(0, current);
        url_path.erase(0, current + 1);

        // Append to path_components depending on what time of component is found
        // Also, add back trailing slash to separate components
        if (is_number(token))
            path_components += "dedupeInt/";
        else if (is_asset(token))
            path_components += "dedupeAsset/";
        else
            path_components += token + "/";
    }

    return path_components;
}

bool Url::is_asset(const std::string &str)
{
    size_t current;
    current = str.find('.');
    if (current == std::string::npos)
        return false;

    std::string extension = str.substr(current, std::string::npos);
    return find(ASSET_EXTENSIONS.begin(), ASSET_EXTENSIONS.end(), extension) != ASSET_EXTENSIONS.end();
}

bool Url::has_extension()
{
    std::filesystem::path fpath {this->path};
    return fpath.has_extension();
}
