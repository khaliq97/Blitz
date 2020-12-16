#include <CSS/Selectors/ComplexSelector.h>
ComplexSelector::ComplexSelector()
{
    compoundSelector = std::make_shared<CompoundSelector>();
}
