#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include <vector>
#include <memory>
#include <JS/Token.h>
class Scanner
{
    public:
        Scanner(std::string source);
        std::string source;
        std::vector<std::shared_ptr<Token>> tokens;
        std::vector<std::shared_ptr<Token>> scanTokens();
        void addToken(TokenType type);
        void addToken(TokenType type, std::string literal);
        void scanToken();
        void identifier();
        void number();
        void string();
        bool match(char expected);
        char peek();
        char peekNext();
        bool isDigit(char c);
        bool isAtEnd();
        char advance();
};

#endif // SCANNER_H
