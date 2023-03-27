#ifndef CPPLOX_EXPRESSION_H
#define CPPLOX_EXPRESSION_H

#include "token.h"

class expression {

};

class bynary_expression : expression {
    public:
        const expression& left;
        const token operator_;
        const expression& right;

        bynary_expression(expression& left, token operator_, expression& right) : left(left), operator_(operator_), right(right) {};
};

class unary_expression : expression {
    public:
        const token operator_;
        const expression& right;

        unary_expression(token operator_, expression& right) : operator_(operator_), right(right) {};
};

class grouping_expression : expression {
    public:
        const expression& expr;

        grouping_expression(expression& expr) : expr(expr) {};
};

class literal_expression : expression {
    public:
//        const literal_value literal;
//        literal_expression(literal_value literal) : literal(literal) {};
};


#endif
