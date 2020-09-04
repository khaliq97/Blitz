#ifndef ELEMENT_H
#define ELEMENT_H
#include <DOM/Node.h>
#include <string>
#include <map>
#include <vector>

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

};
#endif
