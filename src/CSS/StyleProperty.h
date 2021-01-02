#pragma once
#include <CSS/Declaration.h>
class StyleProperty
{
public:
    StyleProperty(const std::shared_ptr<Declaration>& declaration);
    double computedValue;
    std::shared_ptr<Declaration> m_declaration;
private:

};
