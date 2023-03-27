#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H

#include <vector>
#include "token.h"
#include "expression.h"

class parser {
private:
    std::vector<token> tokens;
    int current = 0;
    expression root;

    bool match(std::vector<token> tokens);
    bool is_current_at_end();
    token get_previous_token();

    expression equality();
    expression comparision();
    expression term();
    expression factor();
    expression unary();
    expression primary();


public:
    parser(std::vector<token> tokens) : tokens(tokens) {};
    expression parse();

};


#endif
