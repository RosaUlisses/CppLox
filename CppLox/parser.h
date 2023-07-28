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
    std::vector<std::unique_ptr<statement>> parse();

private:
    std::vector<token> tokens;
    int current = 0;
    bool had_error = false;
    bool parsing_loop = false;
    static constexpr int MAX_ARGUMENT_COUNT = 255;

    
    std::unique_ptr<statement> declaration_stmt();
    std::unique_ptr<statement> block_stmt();
    std::unique_ptr<statement> function_declaration_stmt(); 
    std::unique_ptr<statement> var_declaration_stmt();
    std::unique_ptr<statement> parse_statement();
    std::unique_ptr<statement> if_stmt();
    std::unique_ptr<statement> while_stmt();
    std::unique_ptr<statement> continue_stmt();
    std::unique_ptr<statement> break_stmt();
    std::unique_ptr<statement> return_stmt();
    std::unique_ptr<statement> for_stmt();
    std::unique_ptr<statement> print_stmt();
    std::unique_ptr<statement> expression_stmt();
    
    
    std::unique_ptr<expression> parse_expression();
    std::unique_ptr<expression> anonymous_function();
    std::unique_ptr<expression> assignment();
    std::unique_ptr<expression> ternary();
    std::unique_ptr<expression> logic_or();
    std::unique_ptr<expression> logic_and();
    std::unique_ptr<expression> equality();
    std::unique_ptr<expression> comparision();
    std::unique_ptr<expression> term();
    std::unique_ptr<expression> factor();
    std::unique_ptr<expression> unary();
    std::unique_ptr<expression> call();
    std::unique_ptr<expression> finish_call(std::unique_ptr<expression>& expr);
    std::unique_ptr<expression> primary();
    std::unique_ptr<expression> variable();

    bool match(const std::vector<token_type>& token_types);
    bool is_current_at_end();
    token get_previous_token();
    token consume(token_type type, std::string mensage);
    parse_error generate_parse_error(token token, std::string mensage);
    void report_parsing_error(token token, std::string mensage);
    void report(int line, std::string where, std::string mensage);
};


#endif
