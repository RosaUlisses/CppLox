#ifndef CPPLOX_PARSER_H
#define CPPLOX_PARSER_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include "token.h"
#include "expression.h"
#include "statement.h"

class parser {
public:
    class parse_error : public std::runtime_error {
    public:
        parse_error();
    };
    parser(std::vector<token> tokens);
    std::unique_ptr<expression> parse();

private:
    std::vector<token> tokens;
    int current = 0;
    bool had_error = false;

    
    statement* parse_statement();
    statement* print_stmt();
    statement* expression_stmt();
    
    
    std::unique_ptr<expression> parse_expression();
    std::unique_ptr<expression> ternary();
    std::unique_ptr<expression> equality();
    std::unique_ptr<expression> comparision();
    std::unique_ptr<expression> term();
    std::unique_ptr<expression> factor();
    std::unique_ptr<expression> unary();
    std::unique_ptr<expression> primary();
    std::unique_ptr<expression> variable();

    bool match(const std::vector<token_type>& token_types);
    bool is_current_at_end();
    token get_previous_token();
    void consume(token_type type, std::string mensage);
    parse_error generate_parse_error(token token, std::string mensage);
    void report_parsing_error(token token, std::string mensage);
    void report(int line, std::string where, std::string mensage);
};


#endif
