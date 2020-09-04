#ifndef BOXMODEL_H
#define BOXMODEL_H
#include <string>

class BoxModel
{
    public:
        BoxModel();
        virtual ~BoxModel();

        std::string backgroundColor;

        double paddingTop;
        double paddingBottom;
        double paddingLeft;
        double paddingRight;

        double marginTop;
        double marginBottom;
        double marginLeft;
        double marginRight;

        double borderThickness;

};

#endif // BOXMODEL_H
