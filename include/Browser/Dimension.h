#ifndef DIMENSION_H
#define DIMENSION_H
#include <string>
class Dimension
{
    public:
        Dimension();
        double value { 0 };
        std::string lengthUnit { 0 };
        double renderLength { 0 };
        void setPangoScaleFromLengthUnit(double parentFontSize, double length);
};

#endif // DIMENSION_H
