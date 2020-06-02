//
// Created by Ameen Maali on 6/1/20.
//

#include <regex>
#include "Url.hpp"

Url::Url(const std::string &url, bool regex_mode)
{
    url_string = url;
    if (regex_mode)
    {
        this->regex_parse();
    } else {
        this->parse();
    }
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

const std::string &Url::get_url_string() const {
    return url_string;
}

void Url::set_url_string(const std::string &url_string) {
    Url::url_string = url_string;
}

bool Url::is_encoded(const std::string &u) {
    return u.find('%') != std::string::npos;
}

std::string Url::decode()
{
    std::string ret;
    char ch;
    int i, ii, len = this->url_string.length();

    for (i=0; i < len; i++)
    {
        if (this->url_string[i] != '%')
        {
            if(this->url_string[i] == '+')
            {
                ret += ' ';
            }
            else {
                ret += this->url_string[i];
            }
        } else {
            sscanf(this->url_string.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
    }
    return ret;
}

std::string Url::encode()
{
    std::string encoded_str {};
    char c;
    int ic;
    const char* chars = this->url_string.c_str();
    char bufHex[10];
    int len = strlen(chars);

    for (int i=0; i<len; i++)
    {
        c = chars[i];
        ic = c;

        if (c == ' ')
        {
            encoded_str += '+';
        }
        else {
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
            {
                encoded_str += c;
            }
            else {
                sprintf(bufHex, "%X", c);

                if(ic < 16)
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
    // Decode URLs before parsing
    std::string decoded_url {this->url_string};
    if (Url::is_encoded(decoded_url))
    {
        decoded_url = this->decode();
    }

    std::string parsed_scheme, parsed_hostname, parsed_path, parsed_query_strings, parsed_fragment {};

    std::smatch match;
    if (std::regex_match(decoded_url, match, URL_REGEX))
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
}

bool Url::parse()
{
    // Decode URLs before parsing
    std::string decoded_url {this->url_string};
    if (Url::is_encoded(decoded_url))
    {
        decoded_url = this->decode();
    }

    std::string parsed_scheme, parsed_hostname, parsed_path, parsed_query_strings, parsed_fragment {};

    size_t current {decoded_url.find("://")};
    if (current == std::string::npos)
    {
        return false;
    }

    std::string token {decoded_url.substr(0, current)};
    parsed_scheme = token + "://";

    // Erase the scheme and :// from the current string
    decoded_url.erase(0, current + 3);

    current = decoded_url.find('/');
    // If trailing slash not found, add it
    decoded_url = current == std::string::npos ? decoded_url + '/' : decoded_url;
    current = decoded_url.find('/');

    token = decoded_url.substr(0, current);
    parsed_hostname = token;
    decoded_url.erase(0, parsed_hostname.length());

    current = decoded_url.find('#');
    if (current != std::string::npos)
    {
        token = decoded_url.substr(current + 1, std::string::npos);
        parsed_fragment = token;
        decoded_url.erase(current, std::string::npos);
    }

    current = decoded_url.find('?');
    if (current != std::string::npos)
    {
        token = decoded_url.substr(0, current);
        parsed_path = token;
        decoded_url.erase(0, current + 1);
    } else {
        parsed_path = decoded_url;
        decoded_url.erase(0, current);
    }

    if (!decoded_url.empty())
    {
        parsed_query_strings = decoded_url;
    }

    this->set_hostname(parsed_hostname);
    this->set_path(parsed_path);
    this->set_query_strings(parsed_query_strings);
    this->set_fragment(parsed_fragment);
    return true;
}

std::string Url::get_url_key()
{
    // Decode until all % are gone. If still there after 5 decodes, it most likely indicates
    // a malformed URL and will move on at that point.
    std::string url_key {};
    url_key += this->hostname + this->path;

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
    for (const auto &x: qs_vals) {
        url_key += x + "&";
    }

    return url_key;
}
