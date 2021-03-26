#include <DOM/Element.h>
#include <algorithm>
#include <Parser/Lexer.h>
static Lexer lexer;
Element::Element(std::shared_ptr<Node> parentNode) : Node(parentNode)
{
    namespaceURI = "";
    prefix = "Prefix";
    localName = "";
    tagName = "";
    id = "";
    className = "";
    slot = "";
    attributes = new std::map<std::string, std::string>();
}


bool Element::hasAttributes()
{
    if (attributes)
    {
        if (attributes->size() > 0)
        {
            return true;
        }
    }

    return false;
}

bool Element::hasClass(std::string className)
{
    if (attributes->find("class") != attributes->end())
    {
        std::string tempClassName = "";
        std::string elementClassAttributeValue = attributes->find("class")->second;

        std::vector<std::string> classes;
        for(int i = 0; i < elementClassAttributeValue.length(); i++)
        {
            if (elementClassAttributeValue.at(i) != ' ' && i < elementClassAttributeValue.length() - 1)
            {
                tempClassName += elementClassAttributeValue.at(i);
            }
            else
            {
                if (!(i < elementClassAttributeValue.length() - 1))
                {
                    tempClassName += elementClassAttributeValue.at(elementClassAttributeValue.length() - 1);
                }

                if (tempClassName == className)
                {
                    printf("Matched Class: %s\n", tempClassName.c_str());
                    return true;
                }
                else
                {
                    printf("Ignored Class: %s\n", tempClassName.c_str());
                    tempClassName = "";
                }
            }
        }


    }

    return false;
}

bool Element::doesDeclarationExist(std::string decToFind)
{
    for (auto dec: declarations)
    {
        if (dec->name == decToFind)
        {
            return true;
        }
    }

    return false;
}


bool Element::replaceDeclaration(std::string decToReplace, std::shared_ptr<Declaration> replacementDec)
{
    for (auto dec: declarations)
    {
        if (dec->name == decToReplace)
        {
            removeDeclaration(dec->name);
            declarations.push_back(replacementDec);
            return true;
        }
    }

    return false;
}

std::shared_ptr<StyleProperty> Element::getStylePropertyByDeclarationName(std::string declarationName)
{
    for (auto styleProp: styleProperties)
    {
        if (lexer.caseInsensitiveStringCompare(styleProp->m_declaration->name, declarationName))
        {
            return styleProp;
        }
    }

    return std::make_shared<StyleProperty>();
}

void Element::removeDeclaration(std::string declarationPropertyName)
{
    std::shared_ptr<Declaration> declarationToRemove;
    for (std::shared_ptr<Declaration> dec: declarations)
    {
        if (dec->name == declarationPropertyName)
        {
            declarationToRemove = dec;
            break;
        }
    }

    std::vector<std::shared_ptr<Declaration>>::iterator it = find(declarations.begin(), declarations.end(), declarationToRemove);

    if (it != declarations.end())
    {
        it = declarations.erase(it);
    }
}
