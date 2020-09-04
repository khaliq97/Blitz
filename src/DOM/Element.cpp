#include <DOM/Element.h>

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
