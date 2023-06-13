#ifndef CPPLOX_STATEMENT_H
#define CPPLOX_STATEMENT_H

#include "expression.h"

class var_declaration_statement;
class block_statement;
class if_statement;
class while_statement;
class continue_statement;
class break_statement;
class print_statement;
class expression_statement;

class statement_visitor {
public:
   virtual void visit_var_statement(const var_declaration_statement& statement) = 0;
   virtual void visit_block_statement(const block_statement& statement) = 0;
   virtual void visit_if_statement(const if_statement& statement) = 0;
   virtual void visit_while_statement(const while_statement& statement) = 0;
   virtual void visit_continue_statement(const continue_statement& statement) = 0;
   virtual void visit_break_statement(const break_statement& statement) = 0;
   virtual void visit_print_statement(const print_statement& statement) = 0;
   virtual void visit_expression_statement(const expression_statement& statement) = 0;
   
};

class statement {
public:
    virtual void accept(statement_visitor* visitor) = 0;
};

class var_declaration_statement : public statement {
public:
    var_declaration_statement(token name, std::unique_ptr<expression>& initializer) : name(name), initializer(std::move(initializer)) {}

    void accept(statement_visitor* visitor) override {
        visitor->visit_var_statement(*this);
    }

    token name;
    std::unique_ptr<expression> initializer;
};

class block_statement : public statement {
public:
    block_statement(std::vector<std::unique_ptr<statement>>& statements) : statements(std::move(statements)) {}

    void accept(statement_visitor* visitor) override {
        visitor->visit_block_statement(*this);
    }

    const std::vector<std::unique_ptr<statement>> statements;
};

class if_statement : public statement {
public:
    if_statement(std::unique_ptr<expression>& expr, std::unique_ptr<statement>& then_branch, std::unique_ptr<statement>& else_branch) : expr(std::move(expr)), then_branch(std::move(then_branch)), else_branch(std::move(else_branch)) {}
    
    void accept(statement_visitor* visitor) {
       visitor->visit_if_statement(*this); 
    }
    
    const std::unique_ptr<expression> expr;
    const std::unique_ptr<statement> then_branch;
    const std::unique_ptr<statement> else_branch;
};

class while_statement : public statement {
public:
    while_statement(std::unique_ptr<expression>& expr, std::unique_ptr<statement>& body) : expr(std::move(expr)), body(std::move(body)) {}

    void accept(statement_visitor* visitor) override {
        visitor->visit_while_statement(*this);
    }

    const std::unique_ptr<expression> expr;
    const std::unique_ptr<statement> body;
};

class continue_statement : public statement {
public:
    continue_statement() {}
    
    void accept(statement_visitor* visitor) override {
        visitor->visit_continue_statement(*this);
    }

};

class break_statement : public statement {
public:
    break_statement() {}

    void accept(statement_visitor* visitor) override {
        visitor->visit_break_statement(*this);
    }

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



#endif 
