#ifndef CPPLOX_EXPRESSION_H
#define CPPLOX_EXPRESSION_H

#include "token.h"
#include "lox_value.h"
#include <memory>
#include <any>

class assignment_expression;
class ternary_expression;
class binary_expression;
class unary_expression;
class call_expression;
class grouping_expression;
class literal_expression;
class variable_expression;
class lambda_expression;

class expression_visitor {
public:
    virtual lox_value visit_assignment_expression(const assignment_expression& expression) = 0;
    virtual lox_value visit_ternary_expression(const ternary_expression& expression) = 0;
    virtual lox_value visit_binary_expression(const binary_expression& expression) = 0;
    virtual lox_value visit_unary_expression(const unary_expression& expression) = 0;
    virtual lox_value visit_call_expression(const call_expression& expression) = 0;
    virtual lox_value visit_grouping_expression(const grouping_expression& expression) = 0;
    virtual lox_value visit_literal_expression(const literal_expression& expression) = 0;
    virtual lox_value visit_var_expression(const variable_expression& expression) = 0;
    virtual lox_value visit_lambda_expression(const lambda_expression& expression) = 0;
};


class expression {
public:
    virtual lox_value accept(expression_visitor* visitor) = 0; 
    virtual ~expression()  = default;
};

class assignment_expression : public expression {
public:
    const token name;
    const std::unique_ptr<expression> value;

    assignment_expression(token name, std::unique_ptr<expression>& value) : name(name), value(std::move(value)) {
    };

    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_assignment_expression(*this);
    }
    
    ~assignment_expression() override = default;
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

    ~ternary_expression() override = default;
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


    ~binary_expression() override = default;
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


    ~unary_expression() override = default;
};

class call_expression : public expression {
public:
    const std::unique_ptr<expression> callee;
    const token parenthesis;
    const std::vector<std::unique_ptr<expression>> arguments;
    
    call_expression(std::unique_ptr<expression>& callee, token parenthesis, std::vector<std::unique_ptr<expression>>& arguments) : callee(std::move(callee)),
                                                                                                                parenthesis(parenthesis), arguments(std::move(arguments)) {};
    
    lox_value accept(expression_visitor* visitor) override {
       return visitor->visit_call_expression(*this);
    }
    
    ~call_expression() override = default;
};



class grouping_expression : public expression {
public:
    const std::unique_ptr<expression> expr;

    grouping_expression(std::unique_ptr<expression>& expr) : expr(std::move(expr)) {};

    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_grouping_expression(*this);
    }

    ~grouping_expression() override = default;
};

class literal_expression : public expression {
public:
    const lox_value value;

    literal_expression(lox_value literal) : value(literal) {};

    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_literal_expression(*this);
    }

    ~literal_expression() override = default;
};

class variable_expression : public expression {
public:
    const token name;
    const int distance;

    variable_expression(token name, int distance) : name(name), distance(distance) {};

    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_var_expression(*this);
    }

    ~variable_expression() override = default;
};

class lambda_expression : public expression {
public:
    const std::vector<token> parameters;
    /*
        Due to C++ I could not define the type of body as a std::vector<std::unique_ptr<statement> here. So I decided to define body as a std::vector<std::any>. 
        Given this I save in body values of the type std::unique_ptr<statement>*. With that the interpreter can extract the statements stored in body and execute them without
        getting type-safety problems.
     */
    const std::vector<std::any> body;

    lambda_expression(std::vector<token>& parameters, std::vector<std::any>& body) :  parameters(std::move(parameters)), body(std::move(body)) {};

    lox_value accept(expression_visitor* visitor) override {
        return visitor->visit_lambda_expression(*this);
    }
};

#endif