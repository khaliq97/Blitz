#include <CSS/Selectors/ComplexSelector.h>
ComplexSelector::ComplexSelector()
{
    compoundSelector = std::make_shared<CompoundSelector>();
}

void ComplexSelector::calculateSelectorSpecifity()
{
    if (compoundSelector->typeSelector)
    {
        if (!dynamic_cast<UniversalSelector*>(compoundSelector->typeSelector.get()))
        {
            this->c++;
        }
    }

    for (auto subClassSelctor: compoundSelector->subClassSelectors)
    {
        if (auto idSelector = dynamic_cast<IdSelector*>(subClassSelctor.get()))
        {
            this->a++;
        }
        else if (auto idSelector = dynamic_cast<ClassSelector*>(subClassSelctor.get()))
        {
            // This would also count attribute selectors and pseudo-classes
            this->b++;
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
