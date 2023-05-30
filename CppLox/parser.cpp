#include "parser.h"
#include <algorithm>


parser::parser(std::vector<token> tokens): tokens(tokens) {}

parser::parse_error::parse_error() : runtime_error("") {}

expression parser::parse() {
    try {
        return parse_expression();
    } catch(parse_error& error) {
        return {};
    } 
}

expression parser::parse_expression() {
    return equality();
}

expression parser::equality() {
    auto expression = comparision();
    while (match({EQUAL_EQUAL, BANG_EQUAL})) {
       token operator_ = get_previous_token(); 
       auto right = comparision(); 
       expression = binary_expression(expression, operator_, right);
    }
    
    return expression;
}

expression parser::comparision() {
    auto expression = term();
    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        token operator_ = get_previous_token();
        auto right = term();
        expression = binary_expression(expression, operator_, right);
    }

    return expression;   
}

expression parser::term() {
    auto expression = factor();
    while (match({PLUS, MINUS})) {
        token operator_ = get_previous_token();
        auto right = term();
        expression = binary_expression(expression, operator_, right);
    }

    return expression;
}

expression parser::factor() {
    auto expression = unary();
    while (match({STAR, SLASH})) {
        token operator_ = get_previous_token();
        auto right = term();
        expression = binary_expression(expression, operator_, right);
    }

    return expression;
}

expression parser::unary() {
    if (match({PLUS, MINUS})) {
        token operator_ = get_previous_token(); 
        auto right = unary();
        return unary_expression(operator_, right);
    }
    
    return primary();
}

expression parser::primary() {
    if (match({TRUE})) return literal_expression(true);
    if (match({FALSE})) return literal_expression(false);
    if (match({NIL})) return literal_expression(nullptr);

    if (match({NUMBER, STRING})) {
        return literal_expression(get_previous_token().literal);
    }

    if (match({LEFT_PARENTESIS})) {
        auto expression = parse_expression();
        consume(RIGHT_PARENTESIS, "Expect ')' after expression.");
        return grouping_expression(expression);
    }
    
    throw generate_parse_error(tokens[current], "Expect expression.");
}

void parser::consume(token_type type, std::string mensage) {
    if (tokens[current].type == type) {
        current++;
        return;
    }
    
    throw generate_parse_error(tokens[current], mensage);
}


parser::parse_error parser::generate_parse_error(token token, std::string mensage) {
    report_error(token, mensage);   
    return parse_error();
}

bool parser::match(const std::vector<token_type>& token_list) {
    bool result = std::any_of(token_list.begin(), token_list.end(), [&](const token_type& t){return tokens[current].type == t;});
    current++;
    return result;
}

bool parser::is_current_at_end() {
    return current >= tokens.size();
}

token parser::get_previous_token() {
    return tokens[current - 1];
}