#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <Parser/Lexer.h>
#include <CSS/CSSToken.h>
#include <memory>
class Tokenizer
{
public:
    Tokenizer(std::string css);
    void scanTokens();
    void scanToken();
    bool isAtEnd();
    char currentInputChar();
    char nextInputChar();
    char peek();
    char peekNext();
    //void addToken(CSS::TokenType type);
    bool match(char expected);
    void consume();
    void consumeSpecific(char c);
    void addToken(CSSTokenType type);
    void string();
    void hash();
private:
    std::string source;
    int current;
    std::vector<std::shared_ptr<CSSToken>> tokens;
    bool isNameStartCodePoint(char c);
    bool isNameCodePoint(char c);
    char peekAtOffset(int index);
    bool wouldStartIdentifier(char c1, char c2, char c3);
    std::string consumeName();
    bool wouldStartNumber(char c1, char c2, char c3);
    const std::shared_ptr<Number> consumeNumber();
    std::shared_ptr<CSSToken> createNumericToken();
    void notSupported(std::string tokenName);
};

#endif // TOKENIZER_H
