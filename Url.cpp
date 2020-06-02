//
// Created by Ameen Maali on 6/1/20.
//

#include <regex>
#include "Url.hpp"

Url::Url(std::string &url)
{
    url_string = url;
    this->parse();
}

const std::string &Url::get_scheme() const
{
    return scheme;
}

void Url::set_scheme(const std::string &scheme)
{
    Url::scheme = scheme;
}

const std::string &Url::get_hostname() const
{
    return hostname;
}

void Url::set_hostname(const std::string &hostname)
{
    Url::hostname = hostname;
}

const std::string &Url::get_path() const
{
    return path;
}

void Url::set_path(const std::string &path)
{
    Url::path = path;
}

const std::string &Url::get_query_strings() const
{
    return query_strings;
}

void Url::set_query_strings(const std::string &query_strings)
{
    Url::query_strings = query_strings;
}

const std::string &Url::get_fragment() const
{
    return fragment;
}

void Url::set_fragment(const std::string &fragment)
{
    Url::fragment = fragment;
}

bool Url::is_encoded()
{
    return this->url_string.find('%') != 0;
}

std::string Url::decode()
{
    return std::string();
}

std::string Url::encode()
{
    return std::string();
}

Url Url::parse()
{
    std::string parsed_scheme, parsed_hostname, parsed_path, parsed_query_strings, parsed_fragment {};

    std::smatch match;

    if (std::regex_match(this->url_string, match, URL_REGEX))
    {
        parsed_scheme = match[2].str() + "://";
        parsed_hostname = match[4];
        parsed_path = match[5];
        parsed_query_strings = match[7];
        parsed_fragment = match[9];
    }

    this->set_scheme(parsed_scheme);
    this->set_hostname(parsed_hostname);
    this->set_path(parsed_path);
    this->set_query_strings(parsed_query_strings);
    this->set_fragment(parsed_fragment);

    return *this;
}

std::string Url::get_url_key()
{
    // Decode until all % are gone. If still there after 5 decodes, it most likely indicates
    // a malformed URL and will move on at that point.
    std::string url_key {};
    url_key += this->hostname + this->path;

    int decode_count {0};
    while (this->is_encoded() && decode_count <= 5)
    {
        this->decode();
        decode_count++;
    }

    std::string qs {get_query_strings()};
    if (qs.empty()) {
        return url_key;
    }

    std::string token {};
    size_t current {};
    std::vector<std::string> qs_vals {};
    qs += "&";
    while ((current = qs.find('&')) != std::string::npos) {
        token = qs.substr(0, current);
        qs.erase(0, current + 1);

        std::string qs_key {token.substr(0, token.find('='))};
        qs_vals.push_back(qs_key);
    }

    url_key += "?";
    for (auto &x: qs_vals) {
        url_key += x + "&";
    }

    return url_key;
}

const std::string &Url::get_url_string() const {
    return url_string;
}
