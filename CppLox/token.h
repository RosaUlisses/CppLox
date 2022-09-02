#ifndef CPPLOX_TOKEN_H
#define CPPLOX_TOKEN_H

#include <string>
#include <variant>

typedef std::variant<double,std::string> literal_type;

enum token_type{


};

struct token{

    token_type type;
    std::string lexeme;
    literal_type literal;
    int line;

    token(token_type type, std::string lexeme, literal_type literal, int line);
};

#endif //CPPLOX_TOKEN_H
