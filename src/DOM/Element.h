#pragma once
#include <DOM/Node.h>
#include <string>
#include <map>
#include <vector>
#include <CSS/Declaration.h>
#include <CSS/Selectors/ComplexSelector.h>
#include <CSS/StyleRule.h>
#include <CSS/StyleProperty.h>
class Node;

class Element : public Node
{
public:
    Element(std::weak_ptr<Node> parentNode);

    std::string id = "";
    std::string className = "";
    // TOOD: [SameObject, PutForwards=value] readonly attribute DOMTokenList classList;
    std::string slot = "";

    bool hasAttributes();
    std::map<std::string, std::string> attributes;

    std::vector<std::shared_ptr<CSS::StyleRule>> styleRules;
    std::vector<std::shared_ptr<Declaration>> declarations;

    std::vector<std::shared_ptr<StyleProperty>> styleProperties;

    std::string namespaceURI() const { return m_namespaceURI; }
    std::string prefix() const { return m_prefix; }
    std::string localName() const { return m_localName; }
    std::string tagName() const { return m_tagName; }
    void setTagName(std::string value) { m_tagName = value; }

    void removeDeclaration(std::string declarationPropertyName);
    bool doesDeclarationExist(std::string decToFind);
    bool replaceDeclaration(std::string decToReplace, std::shared_ptr<Declaration> replacementDec);

    std::shared_ptr<StyleProperty> getStylePropertyByDeclarationName(std::string declarationName);

    bool hasClass(std::string className);

    bool isDisplayNone();
    bool isDisplayBlock();

private:
    std::string m_namespaceURI = "";
    std::string m_prefix = "";
    std::string m_localName = "";
    std::string m_tagName = "";
};
