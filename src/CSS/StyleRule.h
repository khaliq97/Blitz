#ifndef STYLERULE_H
#define STYLERULE_H
#include <CSS/Declaration.h>
#include <CSS/Selectors/ComplexSelector.h>
#include <memory>
#include <vector>

namespace CSS {
class StyleRule
{
public:
    StyleRule();
    std::vector<std::shared_ptr<Declaration>> declaration;
    std::vector<std::shared_ptr<ComplexSelector>> complexSelectorList;

};
}


#endif // STYLERULE_H
