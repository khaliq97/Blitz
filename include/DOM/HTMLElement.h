#ifndef HTMLELEMENT_H
#define HTMLELEMENT_H
#include <string>
#include <vector>
#include <DOM/Element.h>

class Element;
class HTMLElement : public Element {
    public:
        HTMLElement(std::shared_ptr<Node> parentNode);

        std::string title;
        std::string lang;
        std::string translate;
        std::string dir;

        bool hidden;
        std::string accessKey;
        std::string accessKeyLabel;
        bool draggable;
        bool spellcheck;
        std::string autoCapitalize;

        // Legacy
        std::string innerText;

};
#endif
