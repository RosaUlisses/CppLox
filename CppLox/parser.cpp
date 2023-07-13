#include "parser.h"
#include <algorithm>


parser::parser(std::vector<token> tokens) : tokens(tokens) {}

parser::parse_error::parse_error() : std::runtime_error("") {}

std::vector<std::unique_ptr<statement>> parser::parse() {
    std::vector<std::unique_ptr<statement>> statements;
    while (!is_current_at_end()) {
        statements.push_back(parse_statement());
    }

    return statements;
}

std::unique_ptr<statement> parser::declaration_stmt() {
    try {
        if (match({VAR})) return var_declaration_stmt();
        if (match({FN})) return function_declaration_stmt();
        

        return parse_statement();
    }
    catch (parse_error &error) {
        had_error = true;
        // todo -> syncronize        
        return nullptr;
    }
}

std::unique_ptr<statement> parser::function_declaration_stmt() {
    token name = consume(IDENTIFIER, "Expect anonymous_function/method name.");
    consume(LEFT_PARENTESIS, "Expect '(' after ");
   
    std::vector<token> parameters;

    do {
        if (parameters.size() > parser::MAX_ARGUMENT_COUNT) {
            report_parsing_error(tokens[current], "Can not have more than " + std::to_string(parser::MAX_ARGUMENT_COUNT) + "parameters.");
        }
        parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
    }while(match({COMMA}));

    consume(SEMICOLON, "Expect ')' after parameters.");

    consume(LEFT_BRACE, "Expect '{' before a function/method body.");
    std::unique_ptr<statement> block = block_stmt();
    
    return std::unique_ptr<statement>(new function_declaration_statement(name, parameters, block));
}

std::unique_ptr<statement> parser::var_declaration_stmt() {
    token name = consume(IDENTIFIER, "Expect variable name.");

    std::unique_ptr<expression> initializer = nullptr;
    if (match({EQUAL})) {
        initializer = parse_expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");

    return std::unique_ptr<statement>(new var_declaration_statement(name, initializer));
}

std::unique_ptr<statement> parser::parse_statement() {
    if (match({IF})) return if_stmt();
    if (match({WHILE})) return while_stmt();
    if (match({CONTINUE})) return continue_stmt();
    if (match({BREAK})) return break_stmt();
    if (match({FOR})) return for_stmt();
    if (match({PRINT})) return print_stmt();
    if (match({LEFT_BRACE})) return block_stmt();

    return expression_stmt();
}

std::unique_ptr<statement> parser::if_stmt() {
    consume(LEFT_PARENTESIS, "Expect '(' after 'if'.");
    auto expr = parse_expression();
    consume(RIGHT_PARENTESIS, "Expect ')' after 'if'.");

    auto then_branch = parse_statement();

    auto else_branch = std::unique_ptr<statement>(nullptr);
    if (match({ELSE})) {
        else_branch = parse_statement();
    }

    return std::unique_ptr<statement>(new if_statement(expr, then_branch, else_branch));
}

std::unique_ptr<statement> parser::while_stmt() {
    parsing_loop = true;
    consume(LEFT_PARENTESIS, "Expect '(' after 'if'.");
    auto expr = parse_expression();
    consume(RIGHT_PARENTESIS, "Expect ')' after 'if'.");

    auto branch = parse_statement();

    parsing_loop = false;
    return std::unique_ptr<statement>(new while_statement(expr, branch));
}

std::unique_ptr<statement> parser::continue_stmt() {
    auto stmt = std::unique_ptr<statement>(new continue_statement());
    consume(SEMICOLON, "Expect ';' after 'continue'.");
    
    if (!parsing_loop) {
        generate_parse_error(get_previous_token(), "Continue statement must be inside a loop.");
    }
    
    return stmt;
}

std::unique_ptr<statement> parser::break_stmt() {
    auto stmt = std::unique_ptr<statement>(new break_statement());
    consume(SEMICOLON, "Expect ';' after 'break'.");

    if (!parsing_loop) {
        generate_parse_error(get_previous_token(), "Break statement must be inside a loop.");
    }

    return stmt;
}

std::unique_ptr<statement> parser::for_stmt() {
    parsing_loop = true;
    consume(LEFT_PARENTESIS, "Expect '(' after 'for'.");

    std::unique_ptr<statement> initializer;
    if (match({SEMICOLON})) {
        initializer = std::unique_ptr<statement>(nullptr);
    } else if (match({VAR})) {
        initializer = var_declaration_stmt();
    } else {
        initializer = expression_stmt();
    }

    auto condition = std::unique_ptr<expression>(new literal_expression(true));
    if (tokens[current].type != SEMICOLON) {
        condition = std::unique_ptr<expression>(parse_expression());
    }
    consume(SEMICOLON, "Expect ';' after 'for' loop condition.");

    auto increment = std::unique_ptr<expression>(nullptr);
    if (tokens[current].type != LEFT_PARENTESIS) {
        increment = parse_expression();
    }

    auto body = std::unique_ptr<statement>(parse_statement());
    if (increment) {
        std::vector<std::unique_ptr<statement>> stmts(2);
        stmts[0] = std::move(body);
        stmts[1] = std::unique_ptr<statement>(new expression_statement(increment));
        body = std::unique_ptr<statement>(new block_statement(stmts));
    }

    auto while_loop = std::unique_ptr<statement>(new while_statement(condition, body));

    if (initializer) {
        std::vector<std::unique_ptr<statement>> stmts(2);
        stmts[0] = std::move(initializer);
        stmts[1] = std::move(while_loop);
        while_loop = std::unique_ptr<statement>(new block_statement(stmts));
    }

    parsing_loop = false;
    return while_loop;
}

std::unique_ptr<statement> parser::block_stmt() {
    std::vector<std::unique_ptr<statement>> statements;
    while (!is_current_at_end() && tokens[current].type != RIGHT_BRACE) {
        statements.push_back(declaration_stmt());
    }

    consume(LEFT_BRACE, "Expect '}' after block.");

    return std::unique_ptr<statement>(new block_statement(statements));
}

std::unique_ptr<statement> parser::print_stmt() {
    auto expression = parse_expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return std::unique_ptr<statement>(new print_statement(expression));
}

std::unique_ptr<statement> parser::expression_stmt() {
    auto expression = parse_expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return std::unique_ptr<statement>(new expression_statement(expression));
}

std::unique_ptr<expression> parser::parse_expression() {
    return assignment();
}

std::unique_ptr<expression> parser::anonymous_function() {
}

std::unique_ptr<expression> parser::assignment() {
    auto expr = ternary();

    if (match({EQUAL})) {
        token equals = get_previous_token();
        auto value = assignment();

        if (typeid(*expr) == typeid(variable_expression)) {
            token name = dynamic_cast<variable_expression *>(expr.get())->name;
            std::unique_ptr<expression>(new assignment_expression(name, value));
        }

        report_parsing_error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<expression> parser::ternary() {
    auto expr = logic_or();
    if (match({INTERROGATION})) {
        auto left_expression = ternary();
        consume(COLON, "Expect ':' in a ternary expression.");
        auto right_expression = ternary();
        expr = std::unique_ptr<expression>(new ternary_expression(expr, left_expression, right_expression));
    }

    return expr;
}

std::unique_ptr<expression> parser::logic_or() {
    auto expr = logic_and();

    if (match({OR})) {
        token operator_ = get_previous_token();
        auto right = logic_and();
        expr = std::unique_ptr<expression>(new binary_expression(expr, operator_, right));
    }

    return expr;
}

std::unique_ptr<expression> parser::logic_and() {
    auto expr = equality();

    if (match({AND})) {
        token operator_ = get_previous_token();
        auto right = equality();
        expr = std::unique_ptr<expression>(new binary_expression(expr, operator_, right));
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

    return call();
}

std::unique_ptr<expression> parser::call() {
    std::unique_ptr<expression> expr = primary();

    while (true) {
        if (match({LEFT_PARENTESIS})) {
            expr = finish_call(expr);
        }
        else break;
    }
    
    return expr;
}

std::unique_ptr<expression> parser::finish_call(std::unique_ptr<expression>& expr) {
    std::vector<std::unique_ptr<expression>> arguments;

    if (tokens[current].type != RIGHT_PARENTESIS) {
        do {
            if (arguments.size() > parser::MAX_ARGUMENT_COUNT) {
                report_parsing_error(tokens[current], "Can not have more than " + std::to_string(parser::MAX_ARGUMENT_COUNT) + "arguments.");
            }
            arguments.push_back(parse_expression());
        } while(match({COMMA}));
    }

    token parenthesis = consume(RIGHT_PARENTESIS, "Expect ')' after arguments.");

    return std::unique_ptr<expression>(new call_expression(expr, parenthesis, arguments));
}

std::unique_ptr<expression> parser::primary() {
    if (match({TRUE})) return std::unique_ptr<expression>(new literal_expression(true));
    if (match({FALSE})) return std::unique_ptr<expression>(new literal_expression(false));
    if (match({NIL})) return std::unique_ptr<expression>(new literal_expression(static_cast<void*>(nullptr)));

    if (match({NUMBER, STRING})) {
        return std::unique_ptr<expression>(new literal_expression(get_previous_token().literal));
    }

    if (match({IDENTIFIER})) {
        return std::unique_ptr<expression>(variable());
    }

    if (match({LEFT_PARENTESIS})) {
        auto expr = parse_expression();
        consume(RIGHT_PARENTESIS, "Expect ')' after expression.");
        return std::unique_ptr<expression>(new grouping_expression(expr));
    }

    throw generate_parse_error(tokens[current], "Expect expression.");
}

std::unique_ptr<expression> parser::variable() {
    return std::unique_ptr<expression>(new variable_expression(get_previous_token()));
}

token parser::consume(token_type type, std::string mensage) {
    if (tokens[current].type == type) {
        current++;
        return get_previous_token();
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
    } else {
        report(token.line, "at '" + token.lexeme + "'", mensage);
    }
}

bool parser::match(const std::vector<token_type> &token_types) {
    bool result = std::any_of(token_types.begin(), token_types.end(),
                              [&](const token_type &t) { return tokens[current].type == t; });
    if (result) current++;
    return result;
}

bool parser::is_current_at_end() {
    return tokens[current].type == END_OF_FILE;
}

token parser::get_previous_token() {
    return tokens[current - 1];
}