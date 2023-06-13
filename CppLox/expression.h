#ifndef CPPLOX_EXPRESSION_H
#define CPPLOX_EXPRESSION_H

#include "token.h"
#include <memory>

class assignment_expression;
class ternary_expression;
class binary_expression;
class unary_expression;
class grouping_expression;
class literal_expression;
class variable_expression;

class expression_visitor {
public:
    virtual lox_value visit_assignment_expression(const assignment_expression& expression) = 0;
    virtual lox_value visit_ternary_expression(const ternary_expression& expression) = 0;
    virtual lox_value visit_binary_expression(const binary_expression& expression) = 0;
    virtual lox_value visit_unary_expression(const unary_expression& expression) = 0;
    virtual lox_value visit_grouping_expression(const grouping_expression& expression) = 0;
    virtual lox_value visit_literal_expression(const literal_expression& expression) = 0;
    virtual lox_value visit_var_expression(const variable_expression& expression) = 0;
};


class expression {
public:
    virtual lox_value accept(expression_visitor* visitor) = 0; 
};

class assignment_expression : public expression {
public:
    const token name;
    const std::unique_ptr<expression> value;

    assignment_expression(token name, std::unique_ptr<expression>& value) : name(name), value(std::move(value)) {
    };

    lox_value accept(expression_visitor* visitor) override {
        visitor->visit_assignment_expression(*this);
    }
};

class ternary_expression : public expression {
public:
    const std::unique_ptr<expression> condition;
    const std::unique_ptr<expression> left;
    const std::unique_ptr<expression> right;

    ternary_expression(std::unique_ptr<expression>& condition, std::unique_ptr<expression>& left, std::unique_ptr<expression>& right) : condition(std::move(condition)), 
                                                                                  left(std::move(left)),
                                                                                  right(std::move(right)) {};
    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_ternary_expression(*this);
    }
};

class binary_expression : public expression {
public:
    const std::unique_ptr<expression> left;
    const token operator_;
    const std::unique_ptr<expression> right;

    binary_expression(std::unique_ptr<expression>& left, token operator_, std::unique_ptr<expression>& right) : left(std::move(left)),
                                                                            operator_(operator_),
                                                                            right(std::move(right)) {
    };

    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_binary_expression(*this);
    }
};


class unary_expression : public expression {
public:
    const token operator_;
    const std::unique_ptr<expression> right;

    unary_expression(token operator_, std::unique_ptr<expression>& right) : operator_(operator_),
                                                           right(std::move(right)) {};
    
    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_unary_expression(*this);
    }
};

class grouping_expression : public expression {
public:
    const std::unique_ptr<expression> expr;

    grouping_expression(std::unique_ptr<expression>& expr) : expr(std::move(expr)) {};

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

class variable_expression : public expression {
public:
    const token name;

    variable_expression(token name) : name(name) {};

    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_var_expression(*this);
    }
};

#endif