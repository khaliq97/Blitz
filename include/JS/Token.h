#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <JS/TokenType.h>
#include <JS/Value.h>
#include <memory>
class Token
{
    public:
        TokenType type;
        std::string lexeme;
        std::shared_ptr<Value> literal;
        int line;

        Token(TokenType type, std::string lexeme, std::shared_ptr<Value> literal, int line);
        std::string toString();

};

#endif // TOKEN_H
