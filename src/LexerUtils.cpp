#include <LexerUtils.h>
std::string Tools::getFileContent(std::string fileName)
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

bool Tools::compareChar(char &c1, char &c2)
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

bool Tools::caseInsensitiveStringCompare(std::string val1, std::string val2)
{
    return val1.size() == val2.size() && std::equal(val1.begin(), val1.end(), val2.begin(), &Tools::compareChar);
}
