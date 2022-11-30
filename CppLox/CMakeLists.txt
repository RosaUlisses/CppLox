cmake_minimum_required(VERSION 3.23)
project(CppLox)

set(CMAKE_CXX_STANDARD 17)

add_executable(CppLox main.cpp lexer.cpp lexer.h token.h)
