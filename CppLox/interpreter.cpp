#include "interpreter.h"

void interpreter::interpret() {
   try {
       for (auto const& statement : statements) {
           execute(statement); 
       }
   } 
   catch(runtime_error& error) {
       report_runtime_error(error);
   }
}

enviroment *interpreter::get_global_enviroment() {
    return global_env.get();
}

void interpreter::report_runtime_error(runtime_error &error) {
    std::cerr << error.what() << "\n[line " << error.token_.line << "]";
}

lox_value interpreter::execute_function(const std::vector<std::unique_ptr<statement>>& body, std::unique_ptr<enviroment>& function_env) {
    std::unique_ptr<enviroment> outscope_env = std::move(env);
    env = std::move(function_env);

    try {
        std::for_each(body.begin(), body.end(), [&](const std::unique_ptr<statement>& stmt) { execute(stmt); });
    }
    catch (return_exception& return_) {
        env = std::move(outscope_env);
        return return_.value;
    }
    
    env = std::move(outscope_env);
    return static_cast<void*>(nullptr);
}

void interpreter::execute(const std::unique_ptr<statement>& statement) {
    statement->accept(this);
}

void interpreter::visit_function_declaration_statement(const function_declaration_statement& statement) {
    lox_function* function = new lox_function(&statement);
    if (!env->has_enclosing()) {
        global_env->declare(statement.name, function);
        return;
    }
    env->declare(statement.name, function);
}


void interpreter::visit_var_declaration_statement(const var_declaration_statement& statement) {
    auto declare_variable = [&](std::unique_ptr<enviroment>& enviroment) {
        if (statement.initializer == nullptr) {
            enviroment->declare(statement.name);
        }
        enviroment->declare(statement.name, this->evaluate(statement.initializer));        
    };  
    
    if (!env->has_enclosing()) declare_variable(global_env);
    else declare_variable(global_env);
}

void interpreter::visit_block_statement(const block_statement& statement) {
    if (should_break || should_continue) return; 
    
    std::unique_ptr<enviroment> previous = std::move(env);
    env = std::make_unique<enviroment>(previous.get());
    
    for (auto const& stmt : statement.statements) {
        if (typeid(*stmt) == typeid(continue_statement)) {
            should_continue = true;
            return;
        }
        if (typeid(*stmt) == typeid(break_statement)) {
            should_break = true;
            return;
        }
        
        execute(stmt); 
    }
     
    env = std::move(previous); 
}

void interpreter::visit_if_statement(const if_statement& statement) {
    if (is_truthy(evaluate(statement.expr))) {
        execute(statement.then_branch);
        return;
    }

    if (statement.else_branch) {
        execute(statement.else_branch);
    }
}

void interpreter::visit_while_statement(const while_statement& statement) {
    while (is_truthy(evaluate(statement.expr))) {
        if (typeid(*(statement.body)) == typeid(continue_statement)) continue;
        if (typeid(*(statement.body)) == typeid(break_statement)) break;
        execute(statement.body);
        if (should_continue) {
            should_continue = false;
        } 
        
        if (should_break) {
            should_break = false;
            break;
        }
    }
}

void interpreter::visit_continue_statement(const continue_statement& statement) {
   should_continue = true; 
}

void interpreter::visit_break_statement(const break_statement& statement) {
    should_break = true; 
}

void interpreter::visit_print_statement(const print_statement& statement) {
    lox_value value = evaluate(statement.expr);   
    std::cout << to_string(value);
}

void interpreter::visit_expression_statement(const expression_statement& statement) {
    evaluate(statement.expr); 
}

void interpreter::visit_return_statement(const return_statement& statement) {
    lox_value value = static_cast<void*>(nullptr);
    if (statement.expr) value = evaluate(statement.expr);
       
    throw return_exception(value);
}

lox_value interpreter::evaluate(const std::unique_ptr<expression>& expression) {
    return expression->accept(this);
}

lox_value interpreter::visit_assignment_expression(const assignment_expression& expression) {
    return env->assign(expression.name, evaluate(expression.value));
}

lox_value interpreter::visit_ternary_expression(const ternary_expression& expression) {
    lox_value left = evaluate(expression.left);
    lox_value right = evaluate(expression.right);
    
    if (is_truthy(evaluate(expression.condition))) {
        return left;
    }
    return right;
}

lox_value interpreter::visit_binary_expression(const binary_expression& expression) {
   lox_value left = evaluate(expression.left);
   lox_value right = evaluate(expression.right);

    switch (expression.operator_.type) {
        case OR:
            return is_truthy(left) || is_truthy(right);

        case AND:
            return is_truthy(left) && is_truthy(right);
        
        case PLUS:
            check_number_or_string_operands(expression.operator_, left, right);
            if (left.index() == lox_types::DOUBLE) return std::get<double>(left) + std::get<double>(right);
            return std::get<std::string>(left) + std::get<std::string>(right);
        
        case MINUS:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) - std::get<double>(right);

        case STAR:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) * std::get<double>(right);

        case SLASH:
            check_number_operands(expression.operator_, left, right);
            if (std::get<double>(right) == 0) {
                throw runtime_error(expression.operator_, "Invalid division, dividing a number by 0.");
            } 
            return std::get<double>(left) / std::get<double>(right);


        case GREATER:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) > std::get<double>(right);

        case LESS:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) < std::get<double>(right);


        case GREATER_EQUAL:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) >= std::get<double>(right);

        case LESS_EQUAL:
            check_number_operands(expression.operator_, left, right);
            return std::get<double>(left) <= std::get<double>(right);

        case EQUAL_EQUAL:
            return left == right;

        case BANG_EQUAL:
            return left != right;
    }
    
    return {};
}

lox_value interpreter::visit_unary_expression(const unary_expression& expression) {
    lox_value right = evaluate(expression.right);
    switch (expression.operator_.type) {
        case MINUS:
            check_number_operand(expression.operator_, right);
            return -std::get<double>(right);
        case BANG:    
            return !is_truthy(right);
    }
    return {}; 
}

lox_value interpreter::visit_call_expression(const call_expression& expression) {
    lox_value callee = evaluate(expression.callee);
    
    std::vector<lox_value> arguments;
    std::for_each(expression.arguments.begin(), expression.arguments.end(), [&](const auto& arg) { arguments.push_back(evaluate(arg)); });

    if (callee.index() != lox_types::LOX_CALLABLE) {
        throw runtime_error(expression.parenthesis, "Can only call functions and classes.");
    }

    lox_callable* function = std::get<lox_callable*>(callee);

    if (arguments.size() != function->arity()) {
        throw runtime_error(expression.parenthesis, "Expect " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments.size()) + " arguments.");
    }
    
    return function->call(*this, arguments);
}


lox_value interpreter::visit_grouping_expression(const grouping_expression& expression) {
    return evaluate(expression.expr);
}

lox_value interpreter::visit_literal_expression(const literal_expression& expression) {
    return expression.value;
}

lox_value interpreter::visit_var_expression(const variable_expression& expression) {
    if (global_env->contains(expression.name)) {
        return global_env->get(expression.name);
    }
    
    return env->get(expression.name);
}

std::string interpreter::to_string(const lox_value &value) {
    switch (value.index()) {
        case lox_types::STRING:
            return std::get<std::string>(value);
            
        case lox_types::DOUBLE: 
            return std::to_string(std::get<double>(value));
            
        case lox_types::BOOL:
            if (std::get<bool>(value)) return "true";
            return "false";
            
        case lox_types::REFERENCE:
            if (std::get<void*>(value) == nullptr) return "nil";
            return "reference";
            
        case lox_types::LOX_CALLABLE:
            return std::get<lox_callable*>(value)->to_string();
            
        default:
            return "";
    } 
}

bool interpreter::is_truthy(const lox_value& value) {
    if (value.index() == lox_types::REFERENCE && std::get<void*>(value) == nullptr) {
        return false;
    }
    if (value.index() == lox_types::BOOL) {
        return std::get<bool>(value);
    }
    
    return true;
}

void interpreter::check_number_or_string_operands(token token_, const lox_value& left, const lox_value& right) {
    if ((left.index() != lox_types::DOUBLE || right.index() != lox_types::DOUBLE) && (left.index() != lox_types::STRING || right.index() != lox_types::STRING) ) {
        throw runtime_error(token_, "Operands must be numbers or strings.");
    }
}

void interpreter::check_number_operands(token token_, const lox_value& left, const lox_value& right) {
    if (left.index() != lox_types::DOUBLE || right.index() != lox_types::DOUBLE) {
       throw runtime_error(token_, "Operands must be numbers.");
    }
}

void interpreter::check_number_operand(token token_, const lox_value& value) {
    if (value.index() != lox_types::DOUBLE) {
        throw runtime_error(token_, "Operand must be a number.");
    }
}