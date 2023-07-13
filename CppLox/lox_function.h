#ifndef CPPLOX_LOX_FUNCTION_H
#define CPPLOX_LOX_FUNCTION_H

#include "interpreter.h"
#include "lox_value.h"
#include "statement.h"
#include "enviroment.h"

class lox_function : public lox_callable {
private:
    const std::unique_ptr<function_declaration_statement> declaration;
    
public:
    lox_function(std::unique_ptr<function_declaration_statement>& declaration): declaration(std::move(declaration)) {
    }
    
    lox_value call(interpreter& interpreter, std::vector<lox_value>& arguments) override {
       enviroment enviroment(interpreter.get_global_enviroment());
        for (int i = 0; i < declaration->parameters.size(); ++i) {
            enviroment.declare(declaration->parameters[i], arguments[i]); 
        }
        
        
        return static_cast<void*>(nullptr);
    }
};

#endif
