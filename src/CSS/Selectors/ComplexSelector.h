#ifndef COMPLEXSELECTOR_H
#define COMPLEXSELECTOR_H
#include <memory>
#include <vector>
#include <CSS/Selectors/CompoundSelector.h>
class ComplexSelector
{
public:
    ComplexSelector();

    // TODO: Add Combinator supports
    std::shared_ptr<CompoundSelector> compoundSelector() { return m_compoundSelector; }

    void calculateSelectorSpecifity();
    bool isThisSelectorMoreSpecific(const std::shared_ptr<ComplexSelector>& complexSelectorCompare);

    std::shared_ptr<CompoundSelector> m_compoundSelector;

    // Selector Specifity
    int a = 0;
    int b = 0;
    int c = 0;

private:
    int m_a = 0;
    int m_b = 0;
    int m_c = 0;

};

#endif // COMPLEXSELECTOR_H
