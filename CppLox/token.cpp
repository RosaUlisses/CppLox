#include "token.h"

token::token(token_type type, std::string lexeme, literal_type literal, int line){
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}
