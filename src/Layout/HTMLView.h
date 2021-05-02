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
    HTMLView(std::shared_ptr<Node> documentWithResolvedStyles);

    std::shared_ptr<DrawCoordinates> drawCoordinates;

    double maxHeight = 0;
    double maxWidth = 0;



private:
    Core* m_core;
    Glib::RefPtr<Pango::Layout> createTextLayout(const std::shared_ptr<Node> renderNode);
    bool isJustWhiteSpace(const std::shared_ptr<Node>& node);
    std::shared_ptr<Node> m_documentWithResolvedStyles;
    std::vector<std::shared_ptr<LayoutBox>> boxes;
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    std::shared_ptr<LayoutBox> root;
    void paintLayoutBox(const Cairo::RefPtr<Cairo::Context>& cr, std::string indent, std::shared_ptr<LayoutBox> layoutBox);
    void createBoxForElement(std::shared_ptr<Node> node, std::shared_ptr<LayoutBox> layoutBox);
    std::shared_ptr<LayoutBox> createBlockBox(std::shared_ptr<Element> elementNode, std::shared_ptr<LayoutBox> parentLayoutBox);
};
