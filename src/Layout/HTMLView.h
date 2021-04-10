#pragma once

#include <gtkmm.h>
#include <Layout/LayoutBox.h>
#include <DOM/Text.h>
class Core;

class DrawCoordinates
{
public:
    int x = 0;
    int y = 0;
};

class HTMLView : public Gtk::DrawingArea
{
public:
    HTMLView(Core* core, const std::shared_ptr<Node> renderElements);

    std::shared_ptr<DrawCoordinates> drawCoordinates;

    double maxHeight = 0;
    double maxWidth = 0;

    void createBoxForElement(std::shared_ptr<Node> node, std::shared_ptr<LayoutBox> layoutBox);
    Glib::RefPtr<Pango::Layout> createTextLayout(const std::shared_ptr<Node> renderNode);
    bool isJustWhiteSpace(const std::shared_ptr<Node>& node);
private:
    Core* m_core;
    std::shared_ptr<Node> renderElements;
    std::vector<std::shared_ptr<LayoutBox>> boxes;
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    std::shared_ptr<LayoutBox> root;
    void paintLayoutBox(const Cairo::RefPtr<Cairo::Context>& cr, std::string indent, std::shared_ptr<LayoutBox> layoutBox);
    std::shared_ptr<LayoutBox> createBlockBox(const std::shared_ptr<Element> elementNode, std::shared_ptr<LayoutBox> parentLayoutBox);
};
