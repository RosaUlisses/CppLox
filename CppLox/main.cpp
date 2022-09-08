#include <iostream>
#include "lexer.h"
#include "token.h"

int main() {
    std::string source =  "if(x >= buceta){\n buceta = x + 1;\n x = \"ulissao\" + 1.234 }";
    std::vector<token> tokens = lexer::scan_tokens(source);
    return 0;
}
