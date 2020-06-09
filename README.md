# urldedupe

urldedupe is a tool to quickly pass in a list of URLs, and get back a list of deduplicated (unique)
URL and query string combination. This is useful to ensure you don't have a URL list will hundreds of duplicated parameters
with differing qs values. For an example run, take the following URL list passed in:

```
https://google.com
https://google.com/home?qs=value
https://google.com/home?qs=secondValue
https://google.com/home?qs=newValue&secondQs=anotherValue
https://google.com/home?qs=asd&secondQs=das
```

Passing through `urldedupe` will only maintain the non-duplicate URL & query string (ignoring values) combinations:

```
$ cat urls.txt | urldedupe
https://google.com
https://google.com/home?qs=value
https://google.com/home?qs=newValue&secondQs=anotherValue
```

It's also possible to deduplicate similar URLs. This is done with `-s|--similar` flag, to deduplicate endpoints such as API endpoints with different IDs, or assets:

```
$ cat urls.txt
https://site.com/api/users/123
https://site.com/api/users/222
https://site.com/api/users/412/profile
https://site.com/users/photos/photo.jpg
https://site.com/users/photos/myPhoto.jpg
https://site.com/users/photos/photo.png
```

Becomes:

```
$ cat urls.txt | urldedupe -s
https://site.com/api/users/123
https://site.com/api/users/412/profile
https://site.com/users/photos/photo.jpg
```

Why C++? Because it's super fast?!?! No not really, I'm working on my C++ skills and mostly just wanted to create a real-world C++ project as opposed to educational related work.

## Installation
Use the binary already compiled within the repository...Or better yet to not run a random binary from myself who can be very shady, compile from source:

You'll need `cmake` installed and C++ 17 or higher.

Clone the repository & navigate to it:
```
git clone https://github.com/ameenmaali/urldedupe.git
cd urldedupe
```

In the `urldedupe` directory
```
cmake CMakeLists.txt
```

If you don't have `cmake` installed, do that. On Mac OS X it is:
```
brew install cmake
```

Run make:
```
make
```

The `urldedupe` binary should now be created in the same directory. For easy use, you can move it to your `bin` directory.

## Usage
`urldedupe` takes URLs from stdin, or a file with the `-u` flag, of which you will most likely want in a file such as:
```
$ cat urls.txt
https://google.com/home/?q=2&d=asd
https://my.site/profile?param1=1&param2=2
https://my.site/profile?param3=3
```

## Help
```
$ ./urldedupe -h
(-h|--help) - Usage/help info for urldedupe
(-u|--urls) - Filename containing urls (use this if you don't pipe urls via stdin)
(-V|--version) - Get current version for urldedupe
(-r|--regex-parse) - This is significantly slower than normal parsing, but may be more thorough or accurate
(-s|--similar) - Remove similar URLs (based on integers and image/font files) - i.e. /api/user/1 & /api/user/2 deduplicated
(-qs|--query-strings-only) - Only include URLs if they have query strings
(-ne|--no-extensions) - Do not include URLs if they have an extension (i.e. .png, .jpg, .woff, .js, .html)
(-m|--mode) - The mode/filters to be enabled (can be 1 or more, comma separated). Default is none, available options are the other flags (--mode "r,s,qs,ne")
```

## Examples

Very simple, simply pass URLs from stdin or with the `-u` flag:

`./urldedupe -u urls.txt`

After moving the `urldedupe` binary to your `bin` dir..Pass in list from stdin and save to a file:

`cat urls.txt | urldedupe > deduped_urls.txt`

Deduplicate similar URLs with `-s|--similar` flag, such as API endpoints with different IDs, or assets:

`cat urls.txt | urldedupe -s`

```
https://site.com/api/users/123
https://site.com/api/users/222
https://site.com/api/users/412/profile
https://site.com/users/photos/photo.jpg
https://site.com/users/photos/myPhoto.jpg
https://site.com/users/photos/photo.png
```

Becomes:

```
https://site.com/api/users/123
https://site.com/api/users/412/profile
https://site.com/users/photos/photo.jpg
```

For all the bug bounty hunters, I recommend chaining with tools such as `waybackurls` or `gau` to get back only unique URLs as those sources are prone to have many similar/duplicated URLs:

`cat waybackurls | urldedupe > deduped_urls.txt`

For max thoroughness (usually not necessary), you can use an RFC complaint regex for URL parsing, but it is significantly slower for large data sets:

`cat urls.txt | urldedupe -r > deduped_urls_regex.txt`

Alternatively, use `-m|--mode` with the flag values you'd like to run with. For example, if you want
to get URLs deduped based on similarity, include only URLs that have query strings, and do not have extensions...

Instead of:

`urldedupe -u urls.txt -s -qs -ne`

You can also do:

`urldedupe -u urls.txt -m "s,qs,ne"`
