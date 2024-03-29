#ifndef CPPLOX_INTERPRETER_H
#define CPPLOX_INTERPRETER_H

#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <stack>
#include "lox_function.h"
#include "expression.h"
#include "statement.h"
#include "error.h"
#include "enviroment.h"
#include "lox_value.h"
#include "native_functions.h"


class interpreter : public expression_visitor, public statement_visitor {
public:
    interpreter(std::vector<std::unique_ptr<statement>> &statements): statements(std::move(statements)), env(new enviroment()) {
        global_env = env.get();
        env->declare("clock", static_cast<lox_callable*>(new lox_clock())); 
    }
    void interpret();
    enviroment* get_global_enviroment();
    lox_value execute_function(const std::vector<std::unique_ptr<statement>>& body, std::unique_ptr<enviroment>& function_env);
    lox_value execute_lambda(const std::vector<std::unique_ptr<statement>*>& body, std::unique_ptr<enviroment>& function_env); 

private:
    enviroment* global_env;
    std::shared_ptr<enviroment> env;
    bool should_continue = false;
    bool should_break = false;
    const std::vector<std::unique_ptr<statement>> statements;
    
    static void report_runtime_error(runtime_error& error);

    void execute(const std::unique_ptr<statement>& statement);
    void visit_function_declaration_statement(const function_declaration_statement& statement) override;
    void visit_var_declaration_statement(const var_declaration_statement& statement) override;
    void visit_block_statement(const block_statement& statement) override;
    void visit_if_statement(const if_statement& statement) override;
    void visit_while_statement(const while_statement& statement) override;
    void visit_continue_statement(const continue_statement& statement) override;
    void visit_break_statement(const break_statement& statement) override;
    void visit_print_statement(const print_statement& statement) override;
    void visit_expression_statement(const expression_statement& statement) override;
    void visit_return_statement(const return_statement& statement) override;
    
    
    lox_value evaluate(const std::unique_ptr<expression>& expression);
   
    lox_value visit_assignment_expression(const assignment_expression& expression) override;
    lox_value visit_ternary_expression(const ternary_expression& expression) override;
    lox_value visit_binary_expression(const binary_expression& expression) override;
    lox_value visit_unary_expression(const unary_expression& expression) override;
    lox_value visit_call_expression(const call_expression& expression) override;
    lox_value visit_grouping_expression(const grouping_expression& expression) override;
    lox_value visit_literal_expression(const literal_expression& expression) override;
    lox_value visit_var_expression(const variable_expression& expression) override;
    lox_value visit_lambda_expression(const lambda_expression& expression) override;


    std::string  to_string(const lox_value& value);
    bool is_truthy(const lox_value& value);
    void check_number_or_string_operands(token token_, const lox_value& left, const lox_value& right);
    void check_number_operand(token token_, const lox_value& left);
    void check_number_operands(token token_, const lox_value& left, const lox_value& right);
};

#endif 