#ifndef BOX_H
#define BOX_H
#include <gtkmm.h>
#include <DOM/Element.h>
#include <CSS/StyleProperty.h>
#include <optional>
class DrawCoordinates;
class HTMLView;

class LayoutBox
{
public:
    LayoutBox(const Glib::RefPtr<Pango::Layout>& pangoLayout, std::shared_ptr<Element> element);

    std::weak_ptr<LayoutBox> parent;
    std::vector<std::shared_ptr<LayoutBox>> children;

    // Font metrics
    double fontSize = 0;

    // Box model metrics
    // TODO: Move into own class that this one inherits from.
    double paddingTop = 0;
    double paddingBottom = 0;
    double paddingLeft = 0;
    double paddingRight = 0;

    double marginTop = 0;
    double marginBottom = 0;
    double marginLeft = 0;
    double marginRight = 0;

    double borderTopWidth = 0;
    double borderBottomWidth = 0;
    double borderLeftWidth = 0;
    double borderRightWidth = 0;

    std::shared_ptr<Color> borderTopColor;
    std::shared_ptr<Color> borderBottomColor;
    std::shared_ptr<Color> borderLeftColor;
    std::shared_ptr<Color> borderRightColor;

    std::shared_ptr<Color> backgroundColor;
    std::shared_ptr<Color> textColor;

    double layoutBoxDrawCursor_X = 0;
    double layoutBoxDrawCursor_Y = 0;

    // TODO: Text node support (line boxes)
    std::shared_ptr<Element> element;
    bool paint(const Cairo::RefPtr<Cairo::Context>& cr);
    void compute();
    void createTextLayout();
    bool isBlockBox();
    bool isInlineBox();
    void updateAllParentHeights();

    Cairo::Rectangle marginRect;
    Cairo::Rectangle borderRect;
    Cairo::Rectangle contentRect;

    bool isRoot = false;


    void appendChildAndUpdateParentHeights(std::shared_ptr<LayoutBox> layoutBox);
private:

    enum Border {
        Top,
        Bottom,
        Left,
        Right
    };



   Glib::RefPtr<Pango::Layout> pangoLayout;
   void drawBorder(Border border, Cairo::Rectangle borderRect, const Cairo::RefPtr<Cairo::Context> &cr);

};

#endif // BOX_H
