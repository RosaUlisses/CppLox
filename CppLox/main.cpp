#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "interpreter.h"

std::string get_source_code() {
    std::ifstream file(R"(C:\Programming\CppLox\CppLox\code.lox)");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    std::string source = get_source_code();
    std::vector<token> tokens = lexer::scan_tokens(source);
    parser parser(tokens);
    std::vector<std::unique_ptr<statement>> statements = parser.parse();
    if (!parser.had_error) {
        interpreter intepreter(statements);
        intepreter.interpret();
    }

    return 0;
}