#ifndef CPPLOX_INTERPRETER_H
#define CPPLOX_INTERPRETER_H

#include <stdexcept>
#include <vector>
#include "expression.h"
#include "statement.h"
#include "error.h"
#include "enviroment.h"


class interpreter : public expression_visitor, public statement_visitor {
public:
    interpreter(std::vector<std::unique_ptr<statement>> &statements): statements(std::move(statements)), env(new enviroment()) {
    }
    void interpret();

private:
    std::unique_ptr<enviroment> env;
    bool should_continue = false;
    bool should_break = false;
    const std::vector<std::unique_ptr<statement>> statements;

    static void report_runtime_error(runtime_error& error);

    void execute(const std::unique_ptr<statement>& statement);
    void visit_var_statement(const var_declaration_statement& statement) override;
    void visit_block_statement(const block_statement& statement) override;
    void visit_if_statement(const if_statement& statement) override;
    void visit_while_statement(const while_statement& statement) override;
    void visit_continue_statement(const continue_statement& statement) override;
    void visit_break_statement(const break_statement& statement) override;
    void visit_print_statement(const print_statement& statement) override;
    void visit_expression_statement(const expression_statement& statement) override;
    
    lox_value evaluate(const std::unique_ptr<expression>& expression);
   
    lox_value visit_assignment_expression(const assignment_expression& expression) override;
    lox_value visit_ternary_expression(const ternary_expression& expression) override;
    lox_value visit_binary_expression(const binary_expression& expression) override;
    lox_value visit_unary_expression(const unary_expression& expression) override;
    lox_value visit_grouping_expression(const grouping_expression& expression) override;
    lox_value visit_literal_expression(const literal_expression& expression) override;
    lox_value visit_var_expression(const variable_expression& expression) override;


    std::string  to_string(const lox_value& value);
    bool is_truthy(const lox_value& value);
    void check_number_or_string_operands(token token_, const lox_value& left, const lox_value& right);
    void check_number_operand(token token_, const lox_value& left);
    void check_number_operands(token token_, const lox_value& left, const lox_value& right);
};

#endif 
