#include "lox_function.h"

lox_value lox_function::call(interpreter& interpreter, std::vector<lox_value>& arguments) {
    std::unique_ptr<enviroment> env = std::make_unique<enviroment>(interpreter.get_global_enviroment());
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

