#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "token.h"
#include "parser.h"
#include <stdlib.h>
#include "interpreter.h"

std::string get_source_code() {
    std::ifstream file(R"(C:\Programming\CppLox\CppLox\code.lox)");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}





int main() {
//    std::string source = get_source_code();
//    std::vector<token> tokens = lexer::scan_tokens(source);
//    auto expr = parser(tokens).parse();
//    interpreter intepreter(expr, <#initializer#>);
//    intepreter.interpret();


    
    
    return 0;
}
