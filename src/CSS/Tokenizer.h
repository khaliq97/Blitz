#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <Parser/Lexer.h>
#include <CSS/CSSToken.h>
#include <memory>
class Tokenizer
{
public:
    Tokenizer(std::string css);
    const std::vector<CSSToken> tokens() { return m_tokens; }
private:
    std::string source;
    int current;
    std::vector<CSSToken> m_tokens;
    void scanTokens();
    void scanToken();
    bool isAtEnd();
    char currentInputChar();
    char nextInputChar();
    char peek();
    char peekNext();
    bool match(char expected);
    void consume();
    void consumeSpecific(char c);
    void addToken(CSSTokenType type);
    void string();
    void hash();
    bool isNameStartCodePoint(char c);
    bool isNameCodePoint(char c);
    char peekAtOffset(int index);
    bool wouldStartIdentifier(char c1, char c2, char c3);
    std::string consumeName();
    bool wouldStartNumber(char c1, char c2, char c3);
    Number consumeNumber();
    CSSToken createNumericToken();
    void notSupported(std::string tokenName);
    void printTokens();
};

#endif // TOKENIZER_H
