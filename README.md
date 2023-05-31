# CppLox
A interpreter for Lox programming language

expression     → ternary ; \
ternary        → equality ( ("?") ternary (":") ternary)* ; \
equality       → comparison ( ( "!=" | "==" ) comparison )* ; \
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ; \
term           → factor ( ( "-" | "+" ) factor )* ; \
factor         → unary ( ( "/" | "*" ) unary )* ; \
unary          → ( "!" | "-" ) unary \ | primary ; \
primary        → NUMBER | STRING | "true" | "false" | "nil"| "(" expression ")" ; \