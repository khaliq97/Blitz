#pragma once
#include <CSS/Declaration.h>
class StyleProperty
{
public:
    StyleProperty();
    StyleProperty(const std::shared_ptr<Declaration>& declaration);
    double computedValue;

    //class Color
    //{
    //public:
    //    Color();
    //    Color(int r, int g, int b);
    //    int r;
    //    int g;
    //    int b;
    //} color;


    std::shared_ptr<Declaration> m_declaration;
private:

};
