//
// Created by Ameen Maali on 6/1/20.
//

#ifndef URLDEDUPE_URL_HPP
#define URLDEDUPE_URL_HPP

#include <string>
#include <regex>
#include <array>

//RFC 3986 Recommendation for URL Regex: https://tools.ietf.org/html/rfc3986#page-51
const std::regex URL_REGEX (R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)", std::regex::extended);
const std::array ASSET_EXTENSIONS {
    ".jpg", ".jpeg", ".png", ".gif", ".tiff",
    ".webm", ".svg", ".eot", ".ttf", ".woff"
};

class Url {
private:
    const std::string url_string;
    std::string scheme;
    std::string hostname;
    std::string path;
    std::string query_strings;
    std::string fragment;
public:
    Url(const std::string &url, bool regex_mode = false);
    ~Url() = default;

    const std::string &get_scheme() const;
    void set_scheme(const std::string &scheme);

    const std::string &get_hostname() const;
    void set_hostname(const std::string &hostname);

    const std::string &get_path() const;
    void set_path(const std::string &path);

    const std::string &get_query_strings() const;
    void set_query_strings(const std::string &query_strings);

    const std::string &get_fragment() const;
    void set_fragment(const std::string &fragment);

    static bool is_encoded(const std::string &u);
    static std::string decode(const std::string&);
    static std::string encode(const std::string&);

    static bool is_image(const std::string &str);

    const std::string &get_url_string() const;

    void regex_parse();
    bool parse();

    std::string get_url_key(bool similar_mode);

    std::string get_path_components() const;
};

#endif //URLDEDUPE_URL_HPP
