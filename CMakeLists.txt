cmake_minimum_required(VERSION 3.23)
project(CppLox)

set(CMAKE_CXX_STANDARD 17)

include_directories(CppLox)

add_executable(CppLox
        CppLox/lexer.cpp
        CppLox/lexer.h
        CppLox/main.cpp
        CppLox/token.cpp
        CppLox/token.h CppLox/token.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")