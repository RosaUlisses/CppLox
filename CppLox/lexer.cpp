#include "lexer.h"

std::vector<token> lexer::scanTokens(std::string source){
    lexer::source = source;
    tokens = std::vector<token>();
    start = 0;
    current = 0;
    line = 1;

    while(!is_current_at_end()){

    }
}

bool lexer::is_current_at_end(){
    return current >= source.length();
}

void lexer::scanToken(){

    switch (source[current])
    {
        case '(' :
            addToken(LEFT_BRACE); 
        break;

        case ')' :
            addToken(RIGHT_BRACE); 
        break;

        case ',' :
            addToken(COMMA); 
        break;

        case ';' :
            addToken(SEMICOLON); 
        break;

        case '.' :
            addToken(LEFT_BRACE); 
        break;

        case '-' :
            addToken(LEFT_BRACE); 
        break;

        case '+' :
            addToken(LEFT_BRACE); 
        break;

        case '*' :
            addToken(LEFT_BRACE); 
        break;

        case '/' :
            addToken(LEFT_BRACE); 
        break;

        case '{' :
            addToken(LEFT_BRACE); 
        break;

        case '}' :
            addToken(LEFT_BRACE); 
        break;

        case '=' :
            if(source[current + 1] == '='){
                current++;
                addToken(EQUAL_EQUAL);
            }
            else addToken(EQUAL); 
        break;

        case '!' :
            if(source[current + 1] == '='){
                current++;
                addToken(BANG_EQUAL);
            }
            else addToken(BANG); 
        break;

        case '<' :
            if(source[current + 1] == '='){
                current++;
                addToken(LESS_EQUAL);
            }
            else addToken(LESS); 
        break;

        case '>' :
            if(source[current + 1] == '='){
                current++;
                addToken(GREATER_EQUAL);
            }
            else addToken(GREATER); 
        break;

        default:
            if(isdigit(source[current])){

            }            
        break;
    }
}

void lexer::addToken(token_type type, literal_type literal = ""){
    std::string lexeme = source.substr(start, current);
    tokens.push_back(token(type, lexeme, literal, line));
}

void lexer::scan_number(){
    
}





