#include <CSS/StyleProperty.h>

StyleProperty::StyleProperty()
{
    color = std::make_shared<Color>();
}


StyleProperty::StyleProperty(const std::shared_ptr<Declaration> &declaration) : m_declaration(declaration)
{
    color = std::make_shared<Color>();
}

Color::Color()
{

}

Color::Color(int r, int g, int b) : r(r), g(g), b(b)
{

}
