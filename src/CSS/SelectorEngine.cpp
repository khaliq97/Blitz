#include <CSS/SelectorEngine.h>
#include <CSS/StyleResolver.h>
SelectorEngine::SelectorEngine(const std::shared_ptr<Stylesheet> &stylesheet, std::shared_ptr<Document> document)
{
    this->stylesheet = stylesheet;
    this->documentWithStyling = document;
    run();
    documentWithStyling->dumpTree(document, "", false);

    std::shared_ptr<StyleResolver> styleResolver = std::make_shared<StyleResolver>(documentWithStyling);
}

void SelectorEngine::applyComplexSelectorToElement(const std::shared_ptr<ComplexSelector> &complexSelector, std::vector<std::shared_ptr<Declaration>> declarations, const std::shared_ptr<Node> &element)
{
    if (auto elementNode = dynamic_cast<Element*>(element.get()))
    {
        if (elementNode->complexSelector)
        {
            for (auto dec: declarations)
            {
                if (elementNode->doesDeclarationExist(dec->name))
                {
                    if (complexSelector->isThisSelectorMoreSpecific(elementNode->complexSelector))
                    {
                        elementNode->complexSelector = std::move(complexSelector);
                        elementNode->replaceDeclaration(dec->name, dec);
                    }
                }
                else
                {
                    elementNode->declarations.push_back(dec);
                }
            }
        }
        else
        {
            elementNode->complexSelector = std::move(complexSelector);
            for (auto dec: declarations)
            {
                elementNode->declarations.push_back(dec);
            }
        }

    }
}

bool SelectorEngine::doAllSelectorsMatchOnAnElement(const std::shared_ptr<ComplexSelector>& complexSelector, const std::shared_ptr<Node>& element)
{
    bool allSelectorsMatchingMatchOnElement = false;

    if (complexSelector->compoundSelector->typeSelector)
    {
        if (matches(complexSelector->compoundSelector->typeSelector, element))
        {
            allSelectorsMatchingMatchOnElement = true;
        }
    }

    for (auto subClassSelector: complexSelector->compoundSelector->subClassSelectors)
    {
        if (matches(subClassSelector, element))
        {
            allSelectorsMatchingMatchOnElement = true;
        }
        else
        {
            allSelectorsMatchingMatchOnElement = false;
            break;
        }
    }

    return allSelectorsMatchingMatchOnElement;
}

void SelectorEngine::run()
{
    std::vector<std::shared_ptr<Node>> empty = {};
    std::vector<std::shared_ptr<Node>> elements = documentWithStyling->getAllNodes(empty, documentWithStyling);

    for (auto styleRule: stylesheet->styleRules)
    {
        for (auto complexSelector: styleRule->complexSelectorList)
        {
            for (auto element: elements)
            {
                if (doAllSelectorsMatchOnAnElement(complexSelector, element))
                {
                     applyComplexSelectorToElement(complexSelector, styleRule->declaration, element);
                }
            }
         }
    }

}

bool SelectorEngine::matches(const std::shared_ptr<SimpleSelector>& simpleSelector, const std::shared_ptr<Node>& element)
{
    if (auto elementNode = dynamic_cast<Element*>(element.get()))
    {
        switch (simpleSelector->type) {
            case SelectorType::Type:
                return simpleSelector->identToken->value() == elementNode->tagName;
            case SelectorType::Universal:
                return true;
            case SelectorType::Id:
                return simpleSelector->identToken->value() == elementNode->id;
            case SelectorType::Class:
                return elementNode->hasClass(simpleSelector->identToken->value());
        }
    }

    return false;
}
