#include <stdio.h>
#include <vector>
#include <string>
#include <DOM/HTMLElement.h>

HTMLElement::HTMLElement(std::shared_ptr<Node> parentNode) : Element(parentNode)
{
    tagName = "";
    title = "";
    lang = "";
    translate = "";
    dir = "";
    hidden = false;
    accessKey = "";
    accessKeyLabel = "";
    draggable = false;
    spellcheck = false;
    autoCapitalize = "";
    innerText = "";
}
