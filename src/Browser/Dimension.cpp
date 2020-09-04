#include <Browser/Dimension.h>
#include <cairo/cairo.h>
#include <gtkmm-3.0/gtkmm.h>

Dimension::Dimension()
{
}

void Dimension::setPangoScaleFromLengthUnit(double parentFontSize, double length)
{
    printf("Length Unit: %s\n", lengthUnit.c_str());
    if (lengthUnit.compare("px") == 0)
    {
        renderLength = value;
    }
    else if (lengthUnit.compare("em") == 0)
    {
        double pangoToEm = parentFontSize * value;
        renderLength = pangoToEm;

    }
}
