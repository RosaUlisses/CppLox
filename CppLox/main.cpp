#include <iostream>
#include "lexer.h"
#include "token.h"

int main() {
    std::string source =  "";
    std::vector<token> tokens = lexer::scan_tokens(source);
    std::cout << "oi";
    return 0;
}
