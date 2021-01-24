#include <CSS/StyleProperty.h>

StyleProperty::StyleProperty()
{
    this->computedValue = 0;
}

StyleProperty::StyleProperty(const std::shared_ptr<Declaration> &declaration) : m_declaration(declaration)
{

}
