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

    bool match(const std::vector<token_type>& token_list);
    bool is_current_at_end();
    token get_previous_token();

    expression parse_expression();
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
