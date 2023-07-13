#ifndef CPPLOX_LOX_VALUE_H
#define CPPLOX_LOX_VALUE_H

#include <string>
#include <variant>
#include <vector>

class interpreter;
class lox_callable;

class lox_types {
public:
    static constexpr int DOUBLE = 0;
    static constexpr int STRING = 1;
    static constexpr int BOOL = 2;
    static constexpr int REFERENCE = 3;
    static constexpr int LOX_CALLABLE = 4;
};

typedef std::variant<double, std::string, bool, void*, lox_callable*> lox_value;

class lox_callable {
public:
    virtual lox_value call(interpreter& interpreter, std::vector<lox_value>& arguments) = 0;
    virtual int arity() = 0;
};

#endif 
