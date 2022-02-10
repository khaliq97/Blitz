#include <LexerUtils.h>
std::string NodeTypes[] =
{
    "ElementNode",
    "Attribute_Node",
    "TextNode",
    "CDATASectionNode",
    "EntityReferenceNode",// Historical
    "EntityNode", // Historical
    "ProcessingInstructionNode",
    "CommentNode",
    "DocumentNode",
    "DocumentTypeNode",
    "DoucmentFragmentNode",
    "NotationNode" // Historical

};

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

std::string Tools::lookupNodeTypeFromDictionary(Node::NodeType nodeType)
{
    return NodeTypes[nodeType - 1];
}

bool Tools::isJustWhiteSpace(const std::shared_ptr<Node>& node)
{
    bool isJustWhiteSpace = false;
    std::string textContent = node->getTextContent();
    for (int i = 0; i < textContent.length(); i++)
    {
        if (textContent.at(i) == ' ' || textContent.at(i) == '\n' || textContent.at(i) == '\t')
        {
            isJustWhiteSpace = true;
        }
        else
        {
            return false;
        }
    }

    return isJustWhiteSpace;
}
