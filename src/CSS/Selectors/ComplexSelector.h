#ifndef COMPLEXSELECTOR_H
#define COMPLEXSELECTOR_H
#include <memory>
#include <vector>
#include <CSS/Selectors/CompoundSelector.h>
class ComplexSelector
{
public:
    ComplexSelector();
    std::shared_ptr<CompoundSelector> compoundSelector;
    // TODO: Add Combinator support


    void calculateSelectorSpecifity();
    bool isThisSelectorMoreSpecific(const std::shared_ptr<ComplexSelector>& complexSelectorCompare);

    // Selector Specifity
    int a = 0;
    int b = 0;
    int c = 0;
};

#endif // COMPLEXSELECTOR_H
