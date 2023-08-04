#ifndef CPPLOX_ENVIROMENT_H
#define CPPLOX_ENVIROMENT_H

#include <unordered_map>
#include <string>
#include <unordered_set>
#include "token.h"
#include "error.h"
#include "lox_value.h"

class enviroment {
private:
    std::unordered_map<std::string, lox_value> values;
    std::unordered_set<std::string> assigned_variables;
    enviroment* enclosing;
    
public:
    enviroment() { enclosing = nullptr; } 
    enviroment(enviroment* enclosing) : enclosing(enclosing) {}

    ~enviroment() {
        for (const auto& [_, value] : values) {
            if (value.index() == lox_types::LOX_CALLABLE) {
                lox_callable* ptr = std::get<lox_callable*>(value); 
                delete ptr;
            } 
        }
    }
    
    bool contains(const token& name) {
        return values.find(name.lexeme) != values.end();
    }
    
    void declare(const token& name, const lox_value value) {
        values.insert({name.lexeme, value});
        assigned_variables.insert(name.lexeme);
    }
    
    void declare(std::string name, const lox_value value) {
        values.insert({name, value});
        assigned_variables.insert(name);
    }
    
    void declare(const token& name) {
        values.insert({name.lexeme, static_cast<void*>(nullptr)});
    }
    
    bool variable_declared(const token& name) {
        return !(values.find(name.lexeme) == values.end()); 
    }
    
    bool variable_not_assigned(const token& name) {
        return assigned_variables.find(name.lexeme) == assigned_variables.end();
    }
    
    lox_value get(const token& name, int distance) {
        if (distance > 0) {
            return enclosing->get(name, distance - 1);
        } 
        
        if (variable_declared(name)) {
            if (variable_not_assigned(name)) {
                throw runtime_error(name, "Accessing unassigned variable '" + name.lexeme + "'.");
            }
            return values[name.lexeme];
        }
        
        throw runtime_error(name, "Undefined variable '" + name.lexeme + "' .");
    }

    lox_value assign(const token& name, const lox_value value) {
        if (variable_declared(name)) {
            if (variable_not_assigned(name)) {
                assigned_variables.insert(name.lexeme);
            }
            return values[name.lexeme] = value;
        }
        
        if (enclosing != nullptr) {
            return enclosing->assign(name, value);
        }

        throw runtime_error(name, "Undefined variable '" + name.lexeme + "' .");
    }
};

#endif 