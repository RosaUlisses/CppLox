#ifndef CPPLOX_EXPRESSION_H
#define CPPLOX_EXPRESSION_H

#include "token.h"
#include <memory>

class expression {
};

class binary_expression : public expression {
    public:
        const std::unique_ptr<expression> left;
        const token operator_;
        const std::unique_ptr<expression> right;

        binary_expression(expression left, token operator_, expression right) : left(std::make_unique<expression>(left)), operator_(operator_), right(std::make_unique<expression>(right)) {
        };
};


class unary_expression : public expression {
    public:
        const token operator_;
        const std::unique_ptr<expression> right;

        unary_expression(token operator_, expression& right) : operator_(operator_), right(std::make_unique<expression>(right)) {};
};

class grouping_expression : public expression {
    public:
        const std::unique_ptr<expression> expr;

        grouping_expression(expression& expr) : expr(std::make_unique<expression>(expr)) {};
};

class literal_expression : public expression {
    public:
        const literal_type literal;
        literal_expression(literal_type literal) : literal(literal) {};
};


#endif
