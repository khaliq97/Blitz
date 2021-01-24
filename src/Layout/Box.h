#ifndef BOX_H
#define BOX_H
#include <gtkmm.h>
#include <DOM/Element.h>
class DrawCoordinates;
class HTMLView;

class Box
{
public:
    Box(const Glib::RefPtr<Pango::Layout>& pangoLayout, const std::shared_ptr<DrawCoordinates>& drawCoords, const std::shared_ptr<Element>& element);

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

    // Box Metrics
    double height = 0;
    double width = 0;

    double contentHeight = 0;
    double contentWidth = 0;

    double contentX = 0;
    double contentY = 0;

    double localDrawX = 0;
    double localDrawY = 0;

    // TODO: Text node support!
    std::shared_ptr<Element> element;
    bool paint(const Cairo::RefPtr<Cairo::Context>& cr);
    void compute();
    void createTextLayout();
    std::shared_ptr<DrawCoordinates> drawCoords;
private:

   Glib::RefPtr<Pango::Layout> pangoLayout;
   HTMLView* htmlView;
};

#endif // BOX_H
