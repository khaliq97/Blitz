#include <JS/Token.h>
Token::Token(TokenType type, std::string lexeme, std::string literal, int line)
{
    this->type = type;
    this->lexeme = lexeme;
    this->value = literal;
    this->line = line;
}

std::string JSTokenTypes[] = {
    "LEFT_PAREN",
    "RIGHT_PAREN",
    "LEFT_BRACE",
    "RIGHT_BRACE",
    "COMMA",
    "DOT",
    "MINUS",
    "PLUS",
    "SEMICOLON",
    "SLASH",
    "STAR",
    "BANG",
    "BANG_EQUAL",
    "EQUAL",
    "EQUAL_EQUAL",
    "GREATER",
    "GREATER_EQUAL",
    "LESS",
    "LESS_EQUAL",
    "IDENTIFIER",
    "STRING",
    "NUMBER",
    "AND",
    "ELSE",
    "FALSE",
    "FUNCTION",
    "FOR",
    "IF",
    "UNDEFINED",
    "NULL",
    "OR",
    "PRINT",
    "RETURN",
    "THIS",
    "TRUE",
    "VAR",
    "WHILE",
    "ENDOFFILE"
};

std::string Token::toString()
{
    return JSTokenTypes[(int) type] + " " + lexeme + " " + value;
}
