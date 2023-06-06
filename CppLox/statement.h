#ifndef CPPLOX_STATEMENT_H
#define CPPLOX_STATEMENT_H

#include "expression.h"

class print_statement;
class expression_statement;
class var_statement;

class statement_visitor {
public:
   virtual void visit_print_statement(const print_statement& statement) = 0;
   virtual void visit_expression_statement(const expression_statement& statement) = 0;
   virtual void visit_var_statement(const var_statement& statement) = 0;
   
};

class statement {
public:
    virtual void accept(statement_visitor* visitor) = 0;
};


class print_statement : public statement {
public:
    print_statement(std::unique_ptr<expression>& expr) : expr(std::move(expr)) {}
    
    void accept(statement_visitor* visitor) override {
        visitor->visit_print_statement(*this);
    }
    
   const std::unique_ptr<expression> expr; 
};

class expression_statement : public statement {
public:
    expression_statement(std::unique_ptr<expression>& expr) : expr(std::move(expr)) {}
    
    void accept(statement_visitor* visitor) override {
        visitor->visit_expression_statement(*this);
    }

    const std::unique_ptr<expression> expr;
};

class var_statement : public statement {
public:
    var_statement(token name, std::unique_ptr<expression>& initializer) : name(name), initializer(std::move(initializer)) {}

    void accept(statement_visitor* visitor) override {
        visitor->visit_var_statement(*this);
    }

    token name;
    std::unique_ptr<expression> initializer;
};

#endif 
