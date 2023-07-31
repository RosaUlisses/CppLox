#include "lox_function.h"

lox_value lox_function::call(interpreter& interpreter, std::vector<lox_value>& arguments) {
    std::unique_ptr<enviroment> env = std::make_unique<enviroment>(closure.get());
    for (int i = 0; i < function_declaration->parameters.size(); ++i) {
        env->declare(function_declaration->parameters[i], arguments[i]); 
    }
    
    return interpreter.execute_function(function_declaration->body, env);
}

int lox_function::arity() {
    return function_declaration->parameters.size();
}

std::string lox_function::to_string() {
    return "<fn " + function_declaration->name.lexeme + ">";
}

lox_value lox_lambda::call(interpreter& interpreter, std::vector<lox_value>& arguments) {
    std::unique_ptr<enviroment> env = std::make_unique<enviroment>(closure.get());

    for (int i = 0; i < lambda_expr->parameters.size(); ++i) {
        env->declare(lambda_expr->parameters[i], arguments[i]);
    } 
    
    std::vector<std::unique_ptr<statement>*> body;
    std::for_each(lambda_expr->body.begin(), lambda_expr->body.end(), [&body] (const std::any& stmt) {
        body.push_back(std::any_cast<std::unique_ptr<statement>*>(stmt));
    });
    
    return interpreter.execute_lambda(body, env);
}

int lox_lambda::arity() {
    return lambda_expr->parameters.size();   
}

std::string  lox_lambda::to_string() {
    return "<fn anonymous>";
}

