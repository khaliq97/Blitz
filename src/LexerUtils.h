#ifndef LEXERUTILS_H
#define LEXERUTILS_H
#include <string>
#include <fstream>
#include <DOM/Node.h>
namespace Tools {

std::string getFileContent(std::string fileName);
bool caseInsensitiveStringCompare(std::string val1, std::string val2);
bool compareChar(char &c1, char &c2);
std::string lookupNodeTypeFromDictionary(Node::NodeType nodeType);
bool isJustWhiteSpace(const std::shared_ptr<Node>& node);

}
#endif // LEXERUTILS_H
