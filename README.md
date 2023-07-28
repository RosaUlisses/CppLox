# CppLox
A interpreter for Lox programming language

program                 →  declaration* EOF ; \
delcaration             →  funcDecl | varDecl | statement ; \
funcDecl                →  "fn" anonymous_function ; \
parameters              →  IDENTIFIER ( "," IDENTIFIER )* ; \
varDecl                 →  "var" IDENTIFIER ("=" expression)? ";" ; \
statement               →  exprStmt | printStmt | ifStmt | whileStmt | continueStmt | breakStmt | forStmt | returnSmt ; \
returnStmt              →  "return" expression? ";" ; \
continueStmt            →  "continue" ";" ; \
breakStmt               →  "break" ";" ; \
ifStmt                  →  "if" "(" expression ")" statement ("else" statement)* ; \
whileStmt               →  "while" "(" expression ")" statement ; \
forStmt                 →  "for" "(" (varDecl | exprStmt ";") expression? ";"  assignment? ")" statement ; \
exprStmt                →  expression ";" ; \
printStmt               →  print expression ";" ; \

expression              → assignment ; \
assignment              → IDENTIFIER "=" assignment | ternary ; \
ternary                 → logic_or ( "?" ternary ":" ternary)* ; \
logicOr                 → logic_and ("or" logic_and)* ; \
logicAnd                → equality ("and" equality)* ; \
equality                → comparison ( ( "!=" | "==" ) comparison )* ; \
comparison              → term ( ( ">" | ">=" | "<" | "<=" ) term )* ; \
term                    → factor ( ( "-" | "+" ) factor )* ; \
factor                  → unary ( ( "/" | "*" ) unary )* ; \
unary                   → ( "!" | "-" ) unary | call | primary ; \
call                    → primary "(" parameters? ")" ; \
parameters               → expression ( "," expression )* ;
primary                 → NUMBER | STRING | "true" | "false" | "nil"| "(" expression ")" ; \