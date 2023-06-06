#ifndef CPPLOX_INTERPRETER_H
#define CPPLOX_INTERPRETER_H

#include <stdexcept>
#include "expression.h"


class interpreter : public expression_visitor {
public:
    interpreter(expression* root): root(root) {}
    void interpret();
    
    class runtime_error : public std::runtime_error {
    public:
        token token_;
        runtime_error(token token_, const std::string &message);
    };   
private:
    expression* root;

    static void report_runtime_error(const interpreter::runtime_error& error); 
    
    lox_value evaluate(const std::unique_ptr<expression>& expression);
   
    lox_value visit_ternary_expression(const ternary_expression& expression) override;
    lox_value visit_binary_expression(const binary_expression& expression) override;
    lox_value visit_unary_expression(const unary_expression& expression) override;
    lox_value visit_grouping_expression(const grouping_expression& expression) override;
    lox_value visit_literal_expression(const literal_expression& expression) override;


    std::string  to_string(const lox_value& value);
    bool is_truthy(const lox_value& value);
    void check_number_or_string_operands(token token_, const lox_value& left, const lox_value& right);
    void check_number_operand(token token_, const lox_value& left);
    void check_number_operands(token token_, const lox_value& left, const lox_value& right);
};

#endif 
