#ifndef HTMLHEADELEMENT_H
#define HTMLHEADELEMENT_H
#include <DOM/HTMLElement.h>
class HTMLHeadElement : public HTMLElement
{
public:
    HTMLHeadElement(std::weak_ptr<Node> parentNode, std::string tagName);
};

#endif // HTMLHEADELEMENT_H
