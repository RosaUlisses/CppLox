#include <iostream>
#include "interpreter.h"

interpreter::runtime_error::runtime_error(token token_, const std::string &message) : token_(token_), std::runtime_error(message) {
}


void interpreter::interpret() {
   try {
       lox_value value = evaluate(root);
       std::cout << to_string(value);
   } 
   catch(runtime_error& error) {
       report_runtime_error(error);
   }
}

void interpreter::report_runtime_error(const interpreter::runtime_error &error) {
    std::cerr << error.what() << "\n[line " << error.token_.line << "]";
}

void interpreter::execute(const std::unique_ptr<statement>& statement) {
    statement->accept(this);
}

void interpreter::visit_print_statement(const print_statement& statement) {
    lox_value value = evaluate(statement.expr);   
    std::cout << to_string(value);
}

void interpreter::visit_expression_statement(const expression_statement& statement) {
    evaluate(statement.expr); 
}

lox_value interpreter::evaluate(const std::unique_ptr<expression>& expression) {
    return expression->accept(this);
}

lox_value interpreter::visit_ternary_expression(const ternary_expression& expression) {
    lox_value left = evaluate(expression.left);
    lox_value right = evaluate(expression.right);
    
    if (is_truthy(evaluate(expression.condition))) {
        return left;
    }
    return right;
}

lox_value interpreter::visit_binary_expression(const binary_expression& expression) {
   lox_value left = evaluate(expression.left);
   lox_value right = evaluate(expression.right);

    switch (expression.operator_.type) {
        case PLUS:
            check_number_or_string_operands(expression.operator_, left, right);
            if (left.index() == lox_types::DOUBLE) return std::get<double>(left) + std::get<double>(right);
            return std::get<std::string>(left) + std::get<std::string>(right);
        
        case MINUS:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) - std::get<double>(right);

        case STAR:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) * std::get<double>(right);

        case SLASH:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) / std::get<double>(right);


        case GREATER:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) > std::get<double>(right);

        case LESS:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) < std::get<double>(right);


        case GREATER_EQUAL:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) >= std::get<double>(right);

        case LESS_EQUAL:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) <= std::get<double>(right);

        case EQUAL_EQUAL:
            return left == right;

        case BANG_EQUAL:
            return left != right;
    }
    
    return {};
}

lox_value interpreter::visit_unary_expression(const unary_expression& expression) {
    lox_value right = evaluate(expression.right);
    switch (expression.operator_.type) {
        case MINUS:
            check_number_operand(expression.operator_, right);
            return -std::get<double>(right);
        case BANG:    
            return !is_truthy(right);
    }
    return {}; 
}

lox_value interpreter::visit_grouping_expression(const grouping_expression& expression) {
    return evaluate(expression.expr);
}

lox_value interpreter::visit_literal_expression(const literal_expression& expression) {
    return expression.value;
}

std::string interpreter::to_string(const lox_value &value) {
    switch (value.index()) {
        case lox_types::STRING:
            return std::get<std::string>(value);
            
        case lox_types::DOUBLE: 
            return std::to_string(std::get<double>(value));
            
        case lox_types::BOOL:
            if (std::get<bool>(value)) return "true";
            return "false";
            
        case lox_types::REFERENCE:
            if (std::get<void*>(value) == nullptr) return "nil";
            return "reference";
    } 
}

bool interpreter::is_truthy(const lox_value& value) {
    if (value.index() == lox_types::REFERENCE && std::get<void*>(value) == nullptr) {
        return false;
    }
    if (value.index() == lox_types::BOOL) {
        return std::get<bool>(value);
    }
    
    return true;
}

void interpreter::check_number_or_string_operands(token token_, const lox_value& left, const lox_value& right) {
    if ((left.index() != lox_types::DOUBLE || right.index() != lox_types::DOUBLE) && (left.index() != lox_types::STRING || right.index() != lox_types::STRING) ) {
        throw runtime_error(token_, "Operands must be numbers or strings.");
    }
}

void interpreter::check_number_operands(token token_, const lox_value& left, const lox_value& right) {
    if (left.index() != lox_types::DOUBLE || right.index() != lox_types::DOUBLE) {
       throw runtime_error(token_, "Operands must be numbers.");
    }
}

void interpreter::check_number_operand(token token_, const lox_value& value) {
    if (value.index() != lox_types::DOUBLE) {
        throw runtime_error(token_, "Operand must be a number.");
    }
}

lox_value interpreter::visit_var_expression(const var_expression &expression) {
    return lox_value();
}

void interpreter::visit_var_statement(const var_statement &statement) {
}


