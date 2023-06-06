#ifndef CPPLOX_EXPRESSION_H
#define CPPLOX_EXPRESSION_H

#include "token.h"
#include <memory>

class ternary_expression;
class binary_expression;
class unary_expression;
class grouping_expression;
class literal_expression;

class expression_visitor {
public:
    virtual lox_value visit_ternary_expression(const ternary_expression& expression) = 0;
    virtual lox_value visit_binary_expression(const binary_expression& expression) = 0;
    virtual lox_value visit_unary_expression(const unary_expression& expression) = 0;
    virtual lox_value visit_grouping_expression(const grouping_expression& expression) = 0;
    virtual lox_value visit_literal_expression(const literal_expression& expression) = 0;
    expression_visitor() {}
};


class expression {
public:
    virtual lox_value accept(expression_visitor* visitor) = 0; 
};

class ternary_expression : public expression {
public:
    const std::unique_ptr<expression> condition;
    const std::unique_ptr<expression> left;
    const std::unique_ptr<expression> right;

    ternary_expression(expression* condition, expression* left, expression* right) : condition(condition), 
                                                                                  left(left),
                                                                                  right(right) {};
    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_ternary_expression(*this);
    }
};

class binary_expression : public expression {
public:
    const std::unique_ptr<expression> left;
    const token operator_;
    const std::unique_ptr<expression> right;

    binary_expression(expression* left, token operator_, expression* right) : left(left),
                                                                            operator_(operator_),
                                                                            right(right) {
    };

    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_binary_expression(*this);
    }
};


class unary_expression : public expression {
public:
    const token operator_;
    const std::unique_ptr<expression> right;

    unary_expression(token operator_, expression* right) : operator_(operator_),
                                                           right(right) {};
    
    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_unary_expression(*this);
    }
};

class grouping_expression : public expression {
public:
    const std::unique_ptr<expression> expr;

    grouping_expression(expression* expr) : expr(expr) {};

    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_grouping_expression(*this);
    }
};

class literal_expression : public expression {
public:
    const lox_value value;

    literal_expression(lox_value literal) : value(literal) {};

    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_literal_expression(*this);
    }
};

#endif