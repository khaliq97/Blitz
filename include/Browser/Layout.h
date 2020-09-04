#ifndef LAYOUT_H
#define LAYOUT_H
#include <Browser/Dimension.h>
#include <optional>
#include <map>
#include <memory>
class Layout
{
    public:
        Layout();
        Layout(Layout*);

        double renderedHeight = 0;
        double renderedWidth = 0;

        std::unique_ptr<Dimension> fontSize;

        std::unique_ptr<Dimension> paddingTop;
        std::unique_ptr<Dimension> paddingBottom;
        std::unique_ptr<Dimension> paddingLeft;
        std::unique_ptr<Dimension> paddingRight;

        std::unique_ptr<Dimension> marginTop;
        std::unique_ptr<Dimension> marginBottom;
        std::unique_ptr<Dimension> marginLeft;
        std::unique_ptr<Dimension> marginRight;

        std::unique_ptr<Dimension> borderThickness;

        std::string display;
};

#endif // LAYOUT_H
