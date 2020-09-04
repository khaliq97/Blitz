#ifndef TERMINAL_H
#define TERMINAL_H
#include <string>
#include <memory>
#include <Parser/Lexer.h>
#include <Lumber.h>
class Terminal
{
    public:
        std::unique_ptr<Lexer> lexer;
        Terminal();
        void start();
        void processCommand();
        std::string consumeUntilChar(char tokenData);
        std::unique_ptr<Lumber> lumberInstance;
    private:

};

#endif // TERMINAL_H
