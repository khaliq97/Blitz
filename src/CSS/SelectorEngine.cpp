#include <CSS/SelectorEngine.h>
#include <CSS/StyleResolver.h>
#include <algorithm>
SelectorEngine::SelectorEngine(const std::shared_ptr<Stylesheet> &stylesheet, std::shared_ptr<Document> document)
{
    this->stylesheet = stylesheet;
    this->documentWithStyling = document;
    run();
    documentWithStyling->dumpTree(document, "", false);

    std::shared_ptr<StyleResolver> styleResolver = std::make_shared<StyleResolver>(documentWithStyling);
}

void SelectorEngine::applyComplexSelectorToElement(const std::shared_ptr<ComplexSelector> &complexSelector, std::shared_ptr<CSS::StyleRule> styleRule, const std::shared_ptr<Node> &element)
{
    if (auto elementNode = dynamic_cast<Element*>(element.get()))
    {
        for (auto dec: styleRule->declaration)
        {
            // Checks if the declaration exist in the elements existing declarations.
            if (elementNode->doesDeclarationExist(dec->name))
            {
                // Loop over all matched style rules for this element.
                for (auto styleRuleInElement: elementNode->styleRules)
                {
                    // Loops over all declarations in all matching style rules.
                    for (auto decInStyleRule: styleRuleInElement->declaration)
                    {
                        // If the the style rules matches the one in the loop then add if it's more specific.
                        if (decInStyleRule->name == dec->name)
                        {
                            if (complexSelector->isThisSelectorMoreSpecific(styleRuleInElement->complexSelectorList[0]))
                            {
                                elementNode->replaceDeclaration(dec->name, dec);
                            }
                        }
                    }
                }



            }
            else
            {
                elementNode->declarations.push_back(dec);
            }

        }

        // Sort the style rules

        elementNode->styleRules.push_back(styleRule);






        //if (elementNode->complexSelector)
        //{
        //    for (auto dec: declarations)
        //    {
        //        if (elementNode->doesDeclarationExist(dec->name))
        //        {
        //            if (complexSelector->isThisSelectorMoreSpecific(elementNode->complexSelector))
        //            {
        //                elementNode->complexSelector = std::move(complexSelector);
        //                elementNode->replaceDeclaration(dec->name, dec);
        //            }
        //        }
        //        else
        //        {
        //            elementNode->declarations.push_back(dec);
        //        }
        //    }
        //}
        //else
        //{
        //    elementNode->complexSelector = std::move(complexSelector);
        //    for (auto dec: declarations)
        //    {
        //        elementNode->declarations.push_back(dec);
        //    }
        //}

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


// NOTE: This is the same logic for checking the selector specifity
//       but instead now works on a style rules a, b and c values.
bool compare(const std::shared_ptr<CSS::StyleRule> l, const std::shared_ptr<CSS::StyleRule>& r)
{
    if (l->a > r->a)
    {
        return true;
    }
    else if (l->a == r->a)
    {
        if (l->b > r->b)
        {
            return true;
        }
        else if (l->b == r->b)
        {
            if (l->c > r->c)
            {
                return true;
            }
            else if (l->c == r->c)
            {
                // Both selectors are equal so just use whatever this one is.
                return true;
            }
        }
    }

    return false;
}


void SelectorEngine::run()
{
    std::vector<std::shared_ptr<Node>> empty = {};
    std::vector<std::shared_ptr<Node>> elements = documentWithStyling->getAllNodes(empty, documentWithStyling);

    // Sort the style rules

    for (auto styleRule: stylesheet->styleRules)
    {
        for (auto complexSelector: styleRule->complexSelectorList)
        {
            styleRule->a += complexSelector->a;
            styleRule->b += complexSelector->b;
            styleRule->c += complexSelector->c;
        }

    }

    std::sort(stylesheet->styleRules.begin(), stylesheet->styleRules.end(), compare);

    for (auto styleRule: stylesheet->styleRules)
    {
        for (auto complexSelector: styleRule->complexSelectorList)
        {
            for (auto element: elements)
            {
                if (doAllSelectorsMatchOnAnElement(complexSelector, element))
                {
                    if (auto elementNode = dynamic_cast<Element*>(element.get()))
                    {
                        applyComplexSelectorToElement(complexSelector, styleRule, element);
                    }

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
