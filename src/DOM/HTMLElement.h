#ifndef HTMLELEMENT_H
#define HTMLELEMENT_H
#include <string>
#include <vector>
#include <DOM/Element.h>

class Element;
class HTMLElement : public Element {
public:
    HTMLElement(std::weak_ptr<Node> parentNode);

    std::string title = "";
    std::string lang = "";
    std::string translate = "";
    std::string dir = "";

    bool hidden = false;
    std::string accessKey = "";
    std::string accessKeyLabel = "";
    bool draggable = false;
    bool spellcheck = false;
    std::string autoCapitalize = "";

    // Legacy
    std::string innerText = "";

};
#endif
