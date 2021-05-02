#include <stdio.h>
#include <vector>
#include <string>
#include <DOM/HTMLElement.h>

HTMLElement::HTMLElement(std::weak_ptr<Node> parentNode) : Element(parentNode)
{

}
