//
// Created by Ameen Maali on 6/1/20.
//

#ifndef URLDEDUPE_URL_HPP
#define URLDEDUPE_URL_HPP

#include <array>
#include <regex>
#include <string>

// RFC 3986 Recommendation for URL Regex: https://tools.ietf.org/html/rfc3986#page-51
const std::regex URL_REGEX(R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)", std::regex::extended);
const std::array ASSET_EXTENSIONS {".jpg", ".jpeg", ".png", ".gif",  ".tiff", ".webm",
                                   ".svg", ".eot",  ".ttf", ".woff", ".ico",  ".woff2"};

class Url
{
  private:
    const std::string url_string;
    std::string_view scheme;
    std::string_view hostname;
    std::string_view path;
    std::string_view query_strings;
    std::string_view fragment;

  public:
    Url(const std::string &url, bool regex_mode = false);
    Url(const Url &);
    Url(Url &&) = default;
    ~Url() = default;

    std::string_view get_scheme() const;
    std::string_view get_hostname() const;
    std::string_view get_path() const;
    std::string_view get_query_strings() const;
    std::string_view get_fragment() const;

    static bool is_encoded(const std::string &);
    static std::string decode(const std::string &);
    static std::string encode(const std::string &);

    static bool is_asset(const std::string &str);

    const std::string &get_url_string() const;

    void regex_parse();
    bool parse();

    std::string get_url_key(bool similar_mode);

    std::string get_path_components() const;

    bool has_extension();
};

#endif // URLDEDUPE_URL_HPP
