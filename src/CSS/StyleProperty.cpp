#include <CSS/StyleProperty.h>
#include <string>
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

Color::Color(double r, double g, double b) : r(r), g(g), b(b)
{
    displayColor = Gdk::RGBA();
    std::string rgbString;

    rgbString.append("rgb(");
    rgbString.append(std::to_string(r));
    rgbString.append(",");
    rgbString.append(std::to_string(g));
    rgbString.append(",");
    rgbString.append(std::to_string(b));
    rgbString.append(")");

    displayColor.set(rgbString);
}
