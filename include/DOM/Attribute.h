#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <string>
#include <Element.h>
#include <optional>
class Attribute
{
    public:
        std::string namespaceURI;
        std::string prefix;
        std::string localName;
        std::string name;
        std::string value;

        Element ownerElement;

        bool specified = true; // useless; always returns true

};

#endif // ATTRIBUTE_H
