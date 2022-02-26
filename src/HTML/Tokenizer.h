#include <string>
#include <HTML/HTMLToken.h>
#include <HTML/Lexer.h>
#include <vector>
#include <LexerUtils.h>

namespace HTML {
class Tokenizer {
public:
    Tokenizer(std::string source);
    void run();
    void printAllTokens();
    std::vector<HTMLToken> tokens() { return m_tokens; }
private:
    HTMLToken::Attribute createAttribute(std::string name, std::string value);
    std::unique_ptr<Lexer> lexer;
    std::vector<HTMLToken> m_tokens;
    void consumeEscapeCodes(bool tab, bool newLine, bool space);
};
}



