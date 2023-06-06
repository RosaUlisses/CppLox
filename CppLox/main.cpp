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

int get(std::unique_ptr<int>& ptr) {
    return *ptr;
}

int main() {
    std::string source = get_source_code();
    std::vector<token> tokens = lexer::scan_tokens(source);
//    interpreter intepreter(parser(tokens).parse());
//    intepreter.interpret();

    std::unique_ptr<int> myptr = std::make_unique<int>(2);
    int x = get(myptr);

    return 0;
}
