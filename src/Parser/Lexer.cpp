#include <stdio.h>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <DOM/HTMLElement.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <Parser/Lexer.h>

std::vector<char> charTokens;

int tokenIndex = 0;
int tokensLength = 0;

std::string Lexer::getFileContent(std::string fileName)
{
    std::string fileContent;

    std::ifstream file;

    file.open(fileName);

    char token[0];
    while(file.peek() != EOF)
    {
        file.read(token, 1);
        fileContent += token[0];
    }

    return fileContent;
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
    return charTokens;
}

bool compareChar(char &c1, char &c2)
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

void Lexer::loadContent(std::string content)
{
    std::string file;
    if (!content.empty())
    {
        for (int i = 0; i < content.length(); i++)
        {
            charTokens.push_back(content.at(i));
            tokensLength++;
        }
    }
}

char Lexer::consumeToken()
{
    char curTok = charTokens[tokenIndex];
    tokenIndex++;
    return curTok;
}

std::string Lexer::peekWhile(std::string tokenData)
{
    std::string returnStr;
    int previousTokenIndex = tokenIndex;
    for (int i = 0; i < tokenData.length(); i++)
    {
        if (charTokens[tokenIndex] == tokenData.at(i))
        {
            returnStr.push_back(charTokens[tokenIndex]);
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
        if (compareChar(charTokens[tokenIndex], tokenData.at(i)))
        {
            returnStr.push_back(charTokens[tokenIndex]);
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
        if (charTokens[tokenIndex] != tokenData.at(i))
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
        if (!(compareChar(charTokens[tokenIndex], tokenData.at(i))))
        {
            return false;
        }

        tokenIndex++;
    }
    return true;
}

void Lexer::consumeToken(char token)
{
    if (charTokens[tokenIndex] == token)
    {
        tokenIndex++;
    }
}


char Lexer::peek()
{
    return charTokens[tokenIndex];
}

char Lexer::peekAtOffset(int offset)
{
    return charTokens[tokenIndex + offset];
}

char Lexer::peekAt(int index)
{
    return charTokens[index];
}

char Lexer::consume()
{
    return charTokens[tokenIndex++];
}

void Lexer::printHTML()
{
    for (auto token: charTokens)
    {
        printf("%c", token);
    }
}

