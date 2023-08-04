cmake_minimum_required(VERSION 3.23)
project(CppLox)

set(CMAKE_CXX_STANDARD 17)

include_directories(CppLox)

add_executable(CppLox
        CppLox/lexer.cpp
        CppLox/lexer.h
        CppLox/main.cpp
        CppLox/token.h CppLox/token.h CppLox/parser.cpp CppLox/parser.h CppLox/expression.h CppLox/interpreter.h CppLox/interpreter.h CppLox/interpreter.cpp CppLox/statement.h CppLox/enviroment.h CppLox/error.h CppLox/lox_value.h CppLox/lox_value.h CppLox/lox_value.h CppLox/native_functions.h CppLox/lox_function.h CppLox/lox_function.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
