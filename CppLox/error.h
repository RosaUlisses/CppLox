#ifndef CPPLOX_ERROR_H
#define CPPLOX_ERROR_H

#include <stdexcept>
#include "token.h"

class runtime_error : public std::runtime_error {
public:
    token token_;
    runtime_error(token token_, const std::string &message) : token_(token_), std::runtime_error(message) { }
};

#endif //CPPLOX_ERROR_H
