#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <ctype.h>
#include "token.h"

class lexer{

public:
    static std::vector<token> scanTokens(std::string source);


private:
    static std::string source;
    static std::vector<token> tokens;
    static int start;
    static int current;
    static int line;

    static bool is_current_at_end();
    static void scanToken();
    static void scan_number();
    static void addToken(token_type type, literal_type literal = "");
};


std::string lexer::source = "";
std::vector<token> lexer::tokens;
int lexer::start = 0;
int lexer::current = 0;
int lexer::line = 1;


#endif