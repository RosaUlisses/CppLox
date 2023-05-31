#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H

#include <vector>
#include <stdexcept>
#include "token.h"
#include "expression.h"
#include "error_handling.h"

class parser {
public:
    class parse_error : public std::runtime_error {
    public:
        parse_error();
    };
    parser(std::vector<token> tokens);
    expression parse();

private:
    std::vector<token> tokens;
    int current = 0;
    bool had_error = false;

    bool match(const std::vector<token_type>& token_types);
    bool is_current_at_end();
    token get_previous_token();

    expression parse_expression();
    expression ternary();
    expression equality();
    expression comparision();
    expression term();
    expression factor();
    expression unary();
    expression primary();
    void consume(token_type type, std::string mensage);
    parse_error generate_parse_error(token token, std::string mensage); 
};


#endif
