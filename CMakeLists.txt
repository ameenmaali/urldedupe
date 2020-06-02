cmake_minimum_required(VERSION 3.16)
project(urldedupe)

set(CMAKE_CXX_STANDARD 17)

add_executable(urldedupe main.cpp utils.cpp flags.cpp flags.hpp utils.hpp Url.cpp Url.hpp)
