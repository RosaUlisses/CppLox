#ifndef CPPLOX_ERROR_H
#define CPPLOX_ERROR_H

#include <stdexcept>
#include "token.h"

class return_exception : public std::runtime_error {
public:
    lox_value value;
    return_exception(lox_value value) : value(value), std::runtime_error("") {}
};

class runtime_error : public std::runtime_error {
public:
    token token_;
    runtime_error(token token_, const std::string &message) : token_(token_), std::runtime_error(message) { }
};

#endif //CPPLOX_ERROR_H
