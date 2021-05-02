#include <CSS/SelectorEngine.h>
#include <CSS/StyleResolver.h>
#include <algorithm>
SelectorEngine::SelectorEngine(std::shared_ptr<Stylesheet>& stylesheet, std::shared_ptr<Document>& document) : m_styleSheet(stylesheet), m_documentWithStyles(document)
{
    run();
}

void SelectorEngine::applyComplexSelectorToElement(const std::shared_ptr<ComplexSelector> &complexSelector, std::shared_ptr<CSS::StyleRule> styleRule, std::weak_ptr<Node> element)
{
    if (element.lock()->isElement())
    {
        auto elementNode = element.lock()->asNodeTypeElement(element);
        for (auto dec: styleRule->declaration)
        {
            // Checks if the declaration exist in the elements existing declarations.
            if (elementNode.lock()->doesDeclarationExist(dec->name))
            {
                // Loop over all matched style rules for this element.
                for (auto styleRuleInElement: elementNode.lock()->styleRules)
                {
                    // Loops over all declarations in all matching style rules.
                    for (auto decInStyleRule: styleRuleInElement->declaration)
                    {
                        // If the the style rules matches the one in the loop then add if it's more specific.
                        if (decInStyleRule->name == dec->name)
                        {
                            if (complexSelector->isThisSelectorMoreSpecific(styleRuleInElement->complexSelectorList[0]))
                            {
                                elementNode.lock()->replaceDeclaration(dec->name, dec);
                            }
                        }
                    }
                }



            }
            else
            {
                elementNode.lock()->declarations.push_back(dec);
            }

        }

        elementNode.lock()->styleRules.push_back(styleRule);
    }
}

bool SelectorEngine::doAllSelectorsMatchOnAnElement(const std::shared_ptr<ComplexSelector>& complexSelector, const std::shared_ptr<Node>& element)
{
    bool allSelectorsMatchingMatchOnElement = false;

    if (complexSelector->m_compoundSelector->m_typeSelector)
    {
        if (matches(complexSelector->m_compoundSelector->m_typeSelector, element))
        {
            allSelectorsMatchingMatchOnElement = true;
        }
    }

    for (auto subClassSelector: complexSelector->m_compoundSelector->m_subClassSelectors)
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
bool compare(const std::shared_ptr<CSS::StyleRule>& l, const std::shared_ptr<CSS::StyleRule>& r)
{
    // FIXME: Sometimes when we have lots of class selctors in the stylesheet,
    // either the left or right style rule is null and causes it to crash. This needs to be investigated.

    if (!l->a || !r->a)
        return false;

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
    std::vector<std::weak_ptr<Node>> empty;
    std::vector<std::weak_ptr<Node>> elements = m_documentWithStyles->getAllNodes(empty, m_documentWithStyles);

    // Adding all complex selector values to their style rule.
    // This value is then used to sort the entire stylesheet most specifc selectors at the top.
    for (auto styleRule: m_styleSheet->styleRules)
    {
        for (auto complexSelector: styleRule->complexSelectorList)
        {
            styleRule->a += complexSelector->a;
            styleRule->b += complexSelector->b;
            styleRule->c += complexSelector->c;
        }

    }

    // Sort the style rules
    std::sort(m_styleSheet->styleRules.begin(), m_styleSheet->styleRules.end(), compare);

    for (auto styleRule: m_styleSheet->styleRules)
    {
        for (auto complexSelector: styleRule->complexSelectorList)
        {
            for (auto element: elements)
            {
                if (doAllSelectorsMatchOnAnElement(complexSelector, element.lock()))
                {
                    if (element.lock()->isElement())
                    {
                        auto elementNode = element.lock()->asNodeTypeElement(element.lock());
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
        switch (simpleSelector->type()) {
            case SelectorType::Type:
                return simpleSelector->value() == elementNode->tagName();
            case SelectorType::Universal:
                return true;
            case SelectorType::Id:
                return simpleSelector->value() == elementNode->id;
            case SelectorType::Class:
                return elementNode->hasClass(simpleSelector->value());
        }
    }

    return false;
}
