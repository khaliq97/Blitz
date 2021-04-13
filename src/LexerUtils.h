#ifndef LEXERUTILS_H
#define LEXERUTILS_H
#include <string>
#include <fstream>
namespace Tools {

std::string getFileContent(std::string fileName);
bool caseInsensitiveStringCompare(std::string val1, std::string val2);
bool compareChar(char &c1, char &c2);

}
#endif // LEXERUTILS_H
