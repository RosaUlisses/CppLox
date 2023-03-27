#include "lexer.h"

std::unordered_map<std::string, token_type> lexer::key_words = {
        {"if", IF},
        {"else", ELSE},
        {"for", FOR},
        {"while", WHILE},
        {"and", AND},
        {"or", OR},
        {"true", TRUE},
        {"false", FALSE},
        {"class", CLASS},
        {"this", THIS},
        {"super", SUPER},
        {"var", VAR},
        {"fun", FUN},
        {"print", PRINT}
};
std::string lexer::source = "";
std::vector<token> lexer::tokens;
int lexer::start = 0;
int lexer::current = 0;
int lexer::line = 1;

std::vector<token> lexer::scan_tokens(std::string source){
    lexer::source = source;
    tokens = std::vector<token>();
    start = 0;
    current = 0;
    line = 1;

    while(!is_current_at_end()){
        start = current;
        scan_token();
    }
    return tokens;
}

void lexer::scan_token(){
    switch (source[current])
    {
        case '(' :
            add_token(LEFT_PARENTESIS);
            current++;
        break;

        case ')' :
            add_token(RIGHT_PARENTESIS);
            current++;
        break;

        case ',' :
            add_token(COMMA);
            current++;
        break;

        case ';' :
            add_token(SEMICOLON);
            current++;
        break;

        case '.' :
            add_token(DOT);
            current++;
        break;

        case '-' :
            add_token(MINUS);
            current++;
        break;

        case '+' :
            add_token(PLUS);
            current++;
        break;

        case '*' :
            add_token(STAR);
            current++;
        break;

        case '/' :
            add_token(SLASH);
            current++;
        break;

        case '{' :
            add_token(LEFT_BRACE);
            current++;
        break;

        case '}' :
            add_token(RIGHT_BRACE);
            current++;
        break;

        case '=' :
            if(source[current + 1] == '='){
                current += 2;
                add_token(EQUAL_EQUAL);
            }
            else{
                add_token(EQUAL);
                current++;
            }
        break;

        case '!' :
            if(source[current + 1] == '='){
                current += 2;
                add_token(BANG_EQUAL);
            }
            else{
                add_token(BANG);
                current++;
            }
        break;

        case '<' :
            if(source[current + 1] == '='){
                current += 2;
                add_token(LESS_EQUAL);
            }
            else{
                add_token(LESS);
                current++;
            }
        break;

        case '>' :
            if(source[current + 1] == '='){
                current += 2;
                add_token(GREATER_EQUAL);
            }
            else{
                add_token(GREATER);
                current++;
            }
        break;

        case '\"' :
            current++;
            scan_string();
        break;

        case '\n' :
           line++;
           current++;
        break;

        default:
            if(source[current] == ' ' || source[current] == '\r' || source[current] == '\t'){
               current++;
            }
            else if(std::isdigit(source[current])){
                scan_number();
            }
            else if(std::isalpha(source[current])){
                scan_identifier();
            }
        break;
    }
}

void lexer::scan_string() {
    while(!is_current_at_end() && source[current] != '\"'){
        current++;
    }
    if(is_current_at_end()){
        return;
    }
    current++;
    std::string string = source.substr(start + 1, current - start - 1);
    add_token(STRING, string);
}

void lexer::scan_identifier() {
    while(!is_current_at_end() && (std::isalpha(source[current]) || std::isdigit(source[current]))){
        current++;
    }
    std::string identifier = source.substr(start, current - start);
    if(key_words.find(identifier) != key_words.end()){
        add_token(key_words[identifier]);
    }
    else{
        add_token(IDENTIFIER, identifier);
    }
}

void lexer::scan_number(){
    while(!is_current_at_end() && std::isdigit(source[current])){
       current++;
    }
    if(source[current] == '.'){
        current++;
        while(!is_current_at_end() && std::isdigit(source[current])) current++;
    }
    double number = std::strtod(source.substr(start, current - start).c_str(), nullptr);
    add_token(NUMBER, number);
}

void lexer::add_token(token_type type,  literal_type literal){
    std::string lexeme = source.substr(start, current == start ? 1 : current - start);
    tokens.push_back(token(type, lexeme, literal, line));
}

bool lexer::is_current_at_end(){
    return current >= source.size();
}