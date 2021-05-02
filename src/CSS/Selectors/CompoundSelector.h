#ifndef COMPOUNDSELECTOR_H
#define COMPOUNDSELECTOR_H
#include <CSS/Selectors/SimpleSelector.h>
#include <memory>
#include <vector>
class CompoundSelector
{
public:
    CompoundSelector();
   // std::shared_ptr<SimpleSelector> typeSelector() { return m_typeSelector; }
   // std::vector<std::shared_ptr<SimpleSelector>> subClassSelectors() { return m_subClassSelectors; }
    std::shared_ptr<SimpleSelector> m_typeSelector;
    std::vector<std::shared_ptr<SimpleSelector>> m_subClassSelectors;
private:


};

#endif // COMPOUNDSELECTOR_H
