cmake_minimum_required(VERSION 3.16)
project(urldedupe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address")

set(SOURCE_FILES main.cpp utils.cpp flags.cpp Url.cpp extern/SpookyV2.cpp)
set(HEADER_FILES utils.hpp flags.hpp Url.hpp extern/SpookyV2.h Hasher.hpp)

add_executable(urldedupe ${SOURCE_FILES} ${HEADER_FILES})
