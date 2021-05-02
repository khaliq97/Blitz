#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
class Lexer {
    public:
        Lexer(std::string source);
        std::vector<char> charStream;

        int tokenIndex = 0;
        int tokensLength = 0;

        void lex();

        char peekNextToken();

        // New Functions
        std::vector<char> getTokens();
        int getTokensLength();
        int getTokenIndex();

        char peek();
        char peekAtOffset(int index);
        char peekAt(int offset);
        std::string peekWhile(std::string tokenData);
        char consume();
        char consumeToken();
        void consumeToken(char token);
        bool consumeTokenWhile(std::string tokenData);
        bool consumeTokenWhileCaseInsesitive(std::string tokenData);
        bool caseInsensitiveStringCompare(std::string val1, std::string val2);
        std::string peekWhileCaseInsesitive(std::string tokenData);
        static bool compareChar(char &c1, char &c2);
};

#endif
