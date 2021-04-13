#ifndef TERMINAL_H
#define TERMINAL_H
#include <string>
#include <memory>
#include <Parser/Lexer.h>
#include <Blitz.h>
class Terminal
{
    public:


        Terminal();
        void start();
        void processCommand();
        std::string consumeUntilChar(char tokenData);
        std::unique_ptr<Blitz> lumberInstance;
    private:
        std::string input;
        int inputIndex = 0;
        std::vector<std::string> tokenStream;

};

#endif // TERMINAL_H
