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
};

#endif // COMPLEXSELECTOR_H
