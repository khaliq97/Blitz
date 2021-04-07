#pragma once
#include <CSS/Declaration.h>
#include <gtkmm.h>
class Color
{
public:
    Color();
    Color(double r, double g, double b);
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;
    Gdk::RGBA displayColor;
};

class StyleProperty
{
public:
    StyleProperty();
    StyleProperty(const std::shared_ptr<Declaration>& declaration);
    double initialValue = 0;
    double computedValue = 0;

    std::shared_ptr<Color> color;

    std::shared_ptr<Declaration> m_declaration;
    StyleProperty(const std::shared_ptr<Declaration> &declaration, double computedValue);
private:

};
