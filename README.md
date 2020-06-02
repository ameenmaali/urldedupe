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
```

## Examples

Very simple, simply pass URLs from stdin or with the `-u` flag:

`./urldedupe -u urls.txt`

After moving the `urldedupe` binary to your `bin` dir..Pass in list from stdin and save to a file:

`cat urls.txt | urldedupe > deduped_urls.txt`

For all the bug bounty hunters, I recommend chaining with tools such as `waybackurls` or `gau` to get back only unique URLs as those sources are prone to have many similar/duplicated URLs:

`cat waybackurls | urldedupe > deduped_urls.txt`
