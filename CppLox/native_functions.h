#ifndef CPPLOX_NATIVE_FUNCTIONS_H
#define CPPLOX_NATIVE_FUNCTIONS_H

#include <chrono>
#include "interpreter.h"
#include "lox_value.h"

class lox_clock : public lox_callable {
private:
    const std::string name = "clock";
public:
    int arity() override {
        return 0;
    }

    lox_value call(interpreter &interpreter, std::vector<lox_value> &arguments) override {
        auto currentTime = std::chrono::duration<double, std::milli>(
                std::chrono::system_clock::now().time_since_epoch()
        ).count();
        return currentTime / 1000;
    }
    
    std::string  to_string() override {
        return "<fn " + name + ">";
    }
};

#endif 
