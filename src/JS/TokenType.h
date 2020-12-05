#ifndef TOKENTYPE_H
#define TOKENTYPE_H
#include <string>
enum class TokenType
{
    // Single character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    AND, ELSE, JSFALSE, FUNCTION, FOR, IF, UNDEFINED, JSNULL, OR,
    PRINT, RETURN, THIS, JSTRUE, VAR, WHILE,

    ENDOFFILE,
};

#endif // TOKENTYPE_H
