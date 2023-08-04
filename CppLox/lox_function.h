#ifndef CPPLOX_LOX_FUNCTION_H
#define CPPLOX_LOX_FUNCTION_H

#include <memory>
#include "interpreter.h"
#include "statement.h"
#include "enviroment.h"
#include "error.h"

class lox_function : public lox_callable {
private:
    const function_declaration_statement* function_declaration;
    const std::shared_ptr<enviroment> closure;
    
public:
    lox_function(const function_declaration_statement* function_declaration, const std::shared_ptr<enviroment>& closure): function_declaration(function_declaration), closure(closure) {}
    
    lox_value call(interpreter& interpreter, std::vector<lox_value>& arguments) override; 
    int arity() override; 
    std::string to_string() override;
};

class lox_lambda : public lox_callable {
private:
    const lambda_expression* lambda_expr;
    const std::shared_ptr<enviroment> closure;

public:    
    lox_lambda(const lambda_expression* lambda_expr, const std::shared_ptr<enviroment>& closure): lambda_expr(lambda_expr), closure(closure) {}

    lox_value call(interpreter& interpreter, std::vector<lox_value>& arguments) override;
    int arity() override;
    std::string to_string() override;
};

#endif