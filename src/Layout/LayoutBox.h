#ifndef BOX_H
#define BOX_H
#include <gtkmm.h>
#include <DOM/Node.h>
#include <CSS/StyleProperty.h>
#include <optional>
#include <Layout/LineBox.h>
class DrawCoordinates;
class HTMLView;

class LayoutBox
{
public:

    enum BlockType {
        Block,
        Inline
    };

    LayoutBox(std::shared_ptr<Node> element, BlockType type);

    std::weak_ptr<LayoutBox> parent;
    std::vector<std::shared_ptr<LayoutBox>> children;

    std::shared_ptr<Element> getElement() { return m_element; }

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
    bool paint(const Cairo::RefPtr<Cairo::Context>& cr);
    void createTextLayout(const Cairo::RefPtr<Cairo::Context>& cairoContext);
    bool isBlockBox();
    bool isInlineBox();
    void updateAllParentHeights();

    Cairo::Rectangle marginRect;
    Cairo::Rectangle borderRect;
    Cairo::Rectangle contentRect;

    bool isRoot = false;

    void appendChildAndUpdateParentHeights(std::shared_ptr<LayoutBox> layoutBox);
    bool isFirstLayoutBoxInContainingBlock(const std::shared_ptr<LayoutBox>& layoutBox);
    std::vector<std::shared_ptr<LayoutBox>> toVectorFromLayoutTree(std::shared_ptr<LayoutBox> initialContainingBlockBox, std::vector<std::shared_ptr<LayoutBox> > layoutVector);
    void enumerate(std::function<void(std::shared_ptr<LayoutBox> box)> callback);
    int calculateYPositionInContainingBlock(std::shared_ptr<LayoutBox> &currentLayoutBox);
private:
    BlockType m_type;
    std::shared_ptr<Element> m_element;

    enum Border {
        Top,
        Bottom,
        Left,
        Right
    };

   Glib::RefPtr<Pango::Layout> m_pangoLayout;

   void drawBorder(Border border, Cairo::Rectangle borderRect, const Cairo::RefPtr<Cairo::Context> &cr);
   std::vector<Layout::LineBox> m_lineBoxes;


};

#endif // BOX_H
