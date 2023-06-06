#include "parser.h"
#include <algorithm>


parser::parser(std::vector<token> tokens): tokens(tokens) {}

parser::parse_error::parse_error() : std::runtime_error("") {}

std::unique_ptr<expression> parser::parse() {
    try {
        return parse_expression();
    }
    catch (parse_error& error) {
        
    }
}

statement* parser::parse_statement() {
    if (match({PRINT})) return print_stmt();
    
    return expression_stmt();
}

statement* parser::print_stmt() {
    auto expression = parse_expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return new print_statement(expression);
}

statement* parser::expression_stmt() {
    auto expression = parse_expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return new expression_statement(expression);
}

std::unique_ptr<expression> parser::parse_expression() {
    return ternary();
}

std::unique_ptr<expression> parser::ternary() {
    auto expr = equality();
    if (match({INTERROGATION})) {
        auto left_expression = ternary();
        consume(COLON, "Expect ':' in a ternary expression.");
        auto right_expression = ternary();
        expr = std::unique_ptr<expression>(new ternary_expression(expr, left_expression, right_expression));
    }
    
    return expr;
}

std::unique_ptr<expression> parser::equality() {
    auto expr = comparision();
    while (match({EQUAL_EQUAL, BANG_EQUAL})) {
       token operator_ = get_previous_token(); 
       auto right = comparision();
        expr = std::unique_ptr<expression>(new binary_expression(expr, operator_, right));
    }
    
    return expr;
}

std::unique_ptr<expression> parser::comparision() {
    auto expr = term();
    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        token operator_ = get_previous_token();
        auto right = term();
        expr = std::unique_ptr<expression>(new binary_expression(expr, operator_, right));
    }

    return expr;   
}

std::unique_ptr<expression> parser::term() {
    auto expr = factor();
    while (match({PLUS, MINUS})) {
        token operator_ = get_previous_token();
        auto right = term();
        expr = std::unique_ptr<expression>(new binary_expression(expr, operator_, right));
    }

    return expr;
}

std::unique_ptr<expression> parser::factor() {
    std::unique_ptr<expression> expr = unary();
    while (match({STAR, SLASH})) {
        token operator_ = get_previous_token();
        auto right = term();
        expr = std::unique_ptr<expression>(new binary_expression(expr, operator_, right));
    }

    return expr;
}

std::unique_ptr<expression> parser::unary() {
    if (match({BANG, MINUS})) {
        token operator_ = get_previous_token(); 
        auto right = unary();
        return std::unique_ptr<expression>(new unary_expression(operator_, right));
    }
    
    return primary();
}

std::unique_ptr<expression> parser::primary() {
    if (match({TRUE})) return std::unique_ptr<expression>(new literal_expression(true));
    if (match({FALSE})) return std::unique_ptr<expression>(new literal_expression(false));
    if (match({NIL})) return std::unique_ptr<expression>(new literal_expression(nullptr));

    if (match({NUMBER, STRING})) {
        return std::unique_ptr<expression>(new literal_expression(get_previous_token().literal));
    }

    if (match({IDENTIFIER})) {}

    if (match({LEFT_PARENTESIS})) {
        auto expr = parse_expression();
        consume(RIGHT_PARENTESIS, "Expect ')' after expression.");
        return std::unique_ptr<expression>(new grouping_expression(expr));
    }
    
    throw generate_parse_error(tokens[current], "Expect expression.");
}

std::unique_ptr<expression> parser::variable() {
    return nullptr;
}

void parser::consume(token_type type, std::string mensage) {
    if (tokens[current].type == type) {
        current++;
        return;
    }
    
    throw generate_parse_error(tokens[current], mensage);
}


parser::parse_error parser::generate_parse_error(token token, std::string mensage) {
    report_parsing_error(token, mensage);   
    return parse_error();
}

void parser::report(int line, std::string where, std::string mensage) {
    std::cout << "[line " << line << "]" << " Error " << where << ": " << mensage;
}

void parser::report_parsing_error(token token, std::string mensage) {
    if (token.type == END_OF_FILE) {
        report(token.line, "at end", mensage);
    }
    else {
        report(token.line, "at '" + token.lexeme + "'", mensage);
    }
}

bool parser::match(const std::vector<token_type>& token_types) {
    bool result = std::any_of(token_types.begin(), token_types.end(), [&](const token_type& t){return tokens[current].type == t;});
    if (result) current++;
    return result;
}

bool parser::is_current_at_end() {
    return current >= tokens.size();
}

token parser::get_previous_token() {
    return tokens[current - 1];
}