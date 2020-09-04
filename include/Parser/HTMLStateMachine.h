#include <string>
#include <Parser/HTMLToken.h>
#include <Parser/Lexer.h>
#include <vector>
class HTMLStateMachine {
    public:

        HTMLStateMachine(std::string content);
        void run();
        void printAllTokens();
        std::vector<std::shared_ptr<HTMLToken>> getParsedTokens() { return tokens; }
    private:
        std::vector<std::shared_ptr<HTMLToken>> tokens;
};


