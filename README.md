# CppLox
A interpreter for Lox programming language

program             →  declaration* EOF ; \
delcaration         →  varDecl | statement ; \
varDecl             →  "var" IDENTIFIER ("=" expression)? ";" ; \
statement           →  exprStmt | printStmt ; \
exprStmt            →  expression ";" ; \
printStmt           →  print expression ";" ; \

expression          → ternary ; \
ternary             → equality ( ("?") ternary (":") ternary)* ; \
equality            → comparison ( ( "!=" | "==" ) comparison )* ; \
comparison          → term ( ( ">" | ">=" | "<" | "<=" ) term )* ; \
term                → factor ( ( "-" | "+" ) factor )* ; \
factor              → unary ( ( "/" | "*" ) unary )* ; \
unary               → ( "!" | "-" ) unary \ | primary ; \
primary             → NUMBER | STRING | "true" | "false" | "nil"| "(" expression ")" ; \