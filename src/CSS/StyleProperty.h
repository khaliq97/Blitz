#pragma once
#include <CSS/Declaration.h>
class Color
{
public:
    Color();
    Color(int r, int g, int b);
    int r = 0;
    int g = 0;
    int b = 0;
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
