#ifndef SELECTORENGINE_H
#define SELECTORENGINE_H
#include <CSS/Selectors/ComplexSelector.h>
#include <CSS/Stylesheet.h>
#include <DOM/Element.h>
#include <DOM/Document.h>
#include <CSS/StyleProperty.h>
class SelectorEngine
{
public:
    SelectorEngine(const std::shared_ptr<Stylesheet>& stylesheet, std::shared_ptr<Document> document);
    std::shared_ptr<Stylesheet> stylesheet;
    std::shared_ptr<Document> documentWithStyling;

private:
    void applyComplexSelectorToElement(const std::shared_ptr<ComplexSelector> &complexSelector, std::shared_ptr<CSS::StyleRule>, const std::shared_ptr<Node> &element);
    void run();
    bool matches(const std::shared_ptr<SimpleSelector>& simpleSelector, const std::shared_ptr<Node> &element);
    bool doAllSelectorsMatchOnAnElement(const std::shared_ptr<ComplexSelector> &complexSelector, const std::shared_ptr<Node> &element);
};

#endif // SELECTORENGINE_H
