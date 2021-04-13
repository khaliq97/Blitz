#include <Terminal/Terminal.h>
#include <Blitz.h>
#include <iostream>
#include <vector>
#include <map>

Terminal::Terminal() { }

enum TerminalToken {
    URL
};

std::map<std::string, TerminalToken> tokenDict = {
    {"url", URL}
};

std::string Terminal::consumeUntilChar(char tokenData)
{
    std::string returnString;
    for (int i = inputIndex; i < input.length(); i++)
    {

        if (input.at(inputIndex) != tokenData)
        {
            returnString += input.at(inputIndex);
        }
        else
        {
            return returnString;
        }
        inputIndex++;

    }

    return returnString;
}

void Terminal::start()
{

        inputIndex = 0;
        input.clear();
        tokenStream.clear();

        printf("lumber web> ");
        std::cin >> input;

        std::string token = consumeUntilChar(' ');
        tokenStream.push_back(token);
        processCommand();

}

void Terminal::processCommand()
{
    for (auto token: tokenStream)
    {
        if (token.compare("url") == 0)
        {
            printf("URL!\n");
        }
        else if (token.compare("quit") == 0)
        {
            exit(0);
        }
        else
        {
            //Todo: run an instance of blitz
        }
    }
}
