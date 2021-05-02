#include <CSS/Selectors/ComplexSelector.h>
ComplexSelector::ComplexSelector()
{
    m_compoundSelector = std::make_shared<CompoundSelector>();
}

void ComplexSelector::calculateSelectorSpecifity()
{
    if (m_compoundSelector->m_typeSelector)
    {
        if (m_compoundSelector->m_typeSelector->type() != SelectorType::Universal)
        {
            this->m_c++;
        }
    }

    for (auto subClassSelctor: m_compoundSelector->m_subClassSelectors)
    {
        if (subClassSelctor->type() == SelectorType::Id)
        {
            this->m_a++;
        }
        else if (subClassSelctor->type() != SelectorType::Class)
        {
            // This would also count attribute selectors and pseudo-classes
            this->m_b++;
        }
    }
}

bool ComplexSelector::isThisSelectorMoreSpecific(const std::shared_ptr<ComplexSelector> &complexSelectorCompare)
{
    if (this->a > complexSelectorCompare->a)
    {
        return true;
    }
    else if (this->a == complexSelectorCompare->a)
    {
        if (this->b > complexSelectorCompare->b)
        {
            return true;
        }
        else if (this->b == complexSelectorCompare->b)
        {
            if (this->c > complexSelectorCompare->c)
            {
                return true;
            }
            else if (this->c == complexSelectorCompare->c)
            {
                // Both selectors are equal so just use whatever this one is.
                return true;
            }
        }
    }

    return false;
}
