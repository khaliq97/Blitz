#include <string>
#include <Parser/HTMLToken.h>
#include <Parser/Lexer.h>
#include <vector>
#include <LexerUtils.h>
class HTMLStateMachine {
public:
    HTMLStateMachine(std::string source);
    void run();
    void printAllTokens();
    std::vector<HTMLToken> tokens() { return m_tokens; }
private:
    HTMLToken::Attribute createAttribute(std::string name, std::string value);
    std::unique_ptr<Lexer> lexer;
    std::vector<HTMLToken> m_tokens;
    void consumeEscapeCodes(bool tab, bool newLine, bool space);
};


