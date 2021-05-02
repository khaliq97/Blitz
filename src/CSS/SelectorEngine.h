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
    SelectorEngine(std::shared_ptr<Stylesheet>& m_styleSheet, std::shared_ptr<Document>& document);

    std::shared_ptr<Document> documentWithStyles() { return m_documentWithStyles; }
private:
    void applyComplexSelectorToElement(const std::shared_ptr<ComplexSelector> &complexSelector, std::shared_ptr<CSS::StyleRule>, std::weak_ptr<Node> element);
    void run();
    bool matches(const std::shared_ptr<SimpleSelector>& simpleSelector, const std::shared_ptr<Node> &element);
    bool doAllSelectorsMatchOnAnElement(const std::shared_ptr<ComplexSelector> &complexSelector, const std::shared_ptr<Node> &element);
    std::shared_ptr<Stylesheet> m_styleSheet;
    std::shared_ptr<Document> m_documentWithStyles;
};

#endif // SELECTORENGINE_H
