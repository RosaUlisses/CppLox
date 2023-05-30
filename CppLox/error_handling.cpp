#include "error_handling.h"

void report(int line, std::string where, std::string mensage) {
   std::cout << "[line " << line << "]" << " Error " << where << ": " << mensage;  
}

void report_error(token token, std::string mensage) {
    if (token.type == END_OF_FILE) {
        report(token.line, "at end", mensage); 
    }
    else {
        report(token.line, "at '" + token.lexeme + "'", mensage); 
    }
}
