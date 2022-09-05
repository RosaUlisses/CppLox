#ifndef CPPLOX_TOKEN_H
#define CPPLOX_TOKEN_H

#include <string>
#include <variant>

typedef std::variant<double,std::string> literal_type;

enum token_type{

    // Single character tokens
    LEFT_PARENTESIS, RIGHT_PARENTESIS, COMMA, 
    SEMICOLON, DOT, MINUS, PLUS, STAR, SLASH, 
    LEFT_BRACE, RIGHT_BRACE, EQUAL, BANG, LESS,
    GREATER,

    // Two character token
    EQUAL_EQUAL, BANG_EQUAL, LESS_EQUAL, GREATER_EQUAL,

    // Literals
    IDENTIFIER, NUMBER, STRING,

    // Keywords
    IF, ELSE, FOR, WHILE, AND, OR, TRUE, FALSE, CLASS, THIS, SUPER,
    VAR, FUN, PRINT,

    END_OF_FILE
};

struct token{

    const token_type type;
    const std::string lexeme;
    const literal_type literal;
    const int line;

    token(token_type type, std::string lexeme, literal_type literal, int line): type(type), lexeme(lexeme), literal(literal), line(line) {};
};

#endif //CPPLOX_TOKEN_H
