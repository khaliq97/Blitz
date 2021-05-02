#include <stdio.h>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <DOM/HTMLElement.h>
#include <stdlib.h>
#include <Parser/Lexer.h>

Lexer::Lexer(std::string source)
{
    if (!source.empty())
    {
        for (int i = 0; i < source.length(); i++)
        {
            charStream.push_back(source.at(i));
            tokensLength++;
        }
    }
}

int Lexer::getTokensLength()
{
    return tokensLength;
}

int Lexer::getTokenIndex()
{
    return tokenIndex;
}

std::vector<char> Lexer::getTokens()
{
    return charStream;
}

bool Lexer::compareChar(char &c1, char &c2)
{
    if(c1 == c2)
    {
        return true;
    }
    else if (std::toupper(c1) == std::toupper(c2))
    {
        return true;
    }

    return false;
}

bool Lexer::caseInsensitiveStringCompare(std::string val1, std::string val2)
{
    return val1.size() == val2.size() && std::equal(val1.begin(), val1.end(), val2.begin(), &compareChar);
}

char Lexer::consumeToken()
{
    char curTok = charStream[tokenIndex];
    tokenIndex++;
    return curTok;
}

std::string Lexer::peekWhile(std::string tokenData)
{
    std::string returnStr;
    int previousTokenIndex = tokenIndex;
    for (int i = 0; i < tokenData.length(); i++)
    {
        if (charStream[tokenIndex] == tokenData.at(i))
        {
            returnStr.push_back(charStream[tokenIndex]);
        }

        tokenIndex++;
    }

    tokenIndex = previousTokenIndex;
    return returnStr;
}

std::string Lexer::peekWhileCaseInsesitive(std::string tokenData)
{
    std::string returnStr;
    int previousTokenIndex = tokenIndex;
    for (int i = 0; i < tokenData.length(); i++)
    {
        if (compareChar(charStream[tokenIndex], tokenData.at(i)))
        {
            returnStr.push_back(charStream[tokenIndex]);
        }

        tokenIndex++;
    }

    tokenIndex = previousTokenIndex;
    return returnStr;
}

bool Lexer::consumeTokenWhile(std::string tokenData)
{
    for (int i = 0; i < tokenData.length(); i++)
    {
        if (charStream[tokenIndex] != tokenData.at(i))
        {
            return false;
        }

        tokenIndex++;
    }

    return true;
}

bool Lexer::consumeTokenWhileCaseInsesitive(std::string tokenData)
{
    for (int i = 0; i < tokenData.length(); i++)
    {
        if (!(compareChar(charStream[tokenIndex], tokenData.at(i))))
        {
            return false;
        }

        tokenIndex++;
    }
    return true;
}

void Lexer::consumeToken(char token)
{
    if (charStream[tokenIndex] == token)
    {
        tokenIndex++;
    }
}


char Lexer::peek()
{
    return charStream[tokenIndex];
}

char Lexer::peekAtOffset(int offset)
{
    return charStream[tokenIndex + offset];
}

char Lexer::peekAt(int index)
{
    return charStream[index];
}

char Lexer::consume()
{
    return charStream[tokenIndex++];
}
