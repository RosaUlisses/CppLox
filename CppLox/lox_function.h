#ifndef CPPLOX_LOX_FUNCTION_H
#define CPPLOX_LOX_FUNCTION_H

#include "interpreter.h"
#include "statement.h"
#include "enviroment.h"

class lox_function : public lox_callable {
private:
    const function_declaration_statement* function_declaration;
    
public:
    lox_function(const function_declaration_statement* function_declaration): function_declaration(function_declaration) {
    }
    
    lox_value call(interpreter& interpreter, std::vector<lox_value>& arguments) override; 
    int arity() override; 
    std::string to_string() override;
};

#endif
