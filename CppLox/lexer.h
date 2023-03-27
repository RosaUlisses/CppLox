#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"
#include <cctype>
#include <iostream>

class lexer{

public:
    static std::vector<token> scan_tokens(std::string source);

private:
    static std::unordered_map<std::string, token_type> key_words;
    static std::string source;
    static std::vector<token> tokens;
    static int start;
    static int current;
    static int line;

    static bool is_current_at_end();
    static void scan_token();
    static void scan_string();
    static void scan_identifier();
    static void scan_number();
    static void add_token(token_type type, literal_type literal = "");
};

#endif