#pragma once
#include <DOM/Node.h>
#include <string>
#include <map>
#include <vector>
#include <CSS/Declaration.h>
#include <CSS/Selectors/ComplexSelector.h>
#include <CSS/StyleProperty.h>
class Node;

class Element : public Node
{
    public:
        Element(std::shared_ptr<Node> parentNode);
        std::string namespaceURI;
        std::string prefix;
        std::string localName;
        std::string tagName;

        std::string id;
        std::string className;
        // TOOD: [SameObject, PutForwards=value] readonly attribute DOMTokenList classList;
        std::string slot;

        bool hasAttributes();
        std::map<std::string, std::string> *attributes;

        std::shared_ptr<ComplexSelector> complexSelector;
        std::vector<std::shared_ptr<Declaration>> declarations;

        std::vector<std::shared_ptr<StyleProperty>> styleProperties;

        void removeDeclaration(std::string declarationPropertyName);
        bool doesDeclarationExist(std::string decToFind);
        bool replaceDeclaration(std::string decToReplace, std::shared_ptr<Declaration> replacementDec);

        bool hasClass(std::string className);
};
