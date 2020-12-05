#ifndef TERMINAL_H
#define TERMINAL_H
#include <string>
#include <memory>
#include <Parser/Lexer.h>
#include <Blitz.h>
class Terminal
{
    public:
        std::unique_ptr<Lexer> lexer;
        Terminal();
        void start();
        void processCommand();
        std::string consumeUntilChar(char tokenData);
        std::unique_ptr<Blitz> lumberInstance;
    private:

};

#endif // TERMINAL_H
