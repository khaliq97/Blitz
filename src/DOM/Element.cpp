#include <DOM/Element.h>
#include <algorithm>
#include <LexerUtils.h>
#include <fmt/core.h>
Element::Element(std::weak_ptr<Node> parentNode) : Node(parentNode)
{
}

bool Element::hasAttributes()
{
    if (attributes.size() > 0)
    {
        return true;
    }

    return false;
}

bool Element::hasClass(std::string className)
{
    if (attributes.find("class") != attributes.end())
    {
        std::string tempClassName = "";
        std::string elementClassAttributeValue = attributes.find("class")->second;

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
                    return true;
                }
                else
                {
                    tempClassName = "";
                }
            }
        }
    }

    return false;
}

bool Element::isDisplayNone()
{
    if (getStylePropertyByDeclarationName("display"))
        return getStylePropertyByDeclarationName("display")->m_declaration->value[0].value() == "none";

    return false;
}

bool Element::isDisplayBlock()
{
    if (getStylePropertyByDeclarationName("display"))
        return getStylePropertyByDeclarationName("display")->m_declaration->value[0].value() == "block";

    return false;
}

bool Element::isDisplayInline()
{
    if (getStylePropertyByDeclarationName("display"))
        return getStylePropertyByDeclarationName("display")->m_declaration->value[0].value() == "inline";

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
        if (Tools::caseInsensitiveStringCompare(styleProp->m_declaration->name, declarationName))
        {
            return styleProp;
        }
    }

    return std::make_shared<StyleProperty>();
}

std::shared_ptr<Node> Element::getDOMNode()
{
    return this->shared_from_this();
}

std::optional<std::shared_ptr<LayoutBox>> Element::createLayoutBox()
{
    if (isDisplayNone())
        return {};

    if (isDisplayBlock())
    {
        return std::make_shared<LayoutBox>(this->getDOMNode(), LayoutBox::BlockType::Block);
    }
    else if (isDisplayInline())
    {
        return std::make_shared<LayoutBox>(this->getDOMNode(), LayoutBox::BlockType::Inline);
    }

    return {};
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
