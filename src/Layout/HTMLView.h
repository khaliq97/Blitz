#pragma once

#include <gtkmm.h>
#include <Layout/LayoutBox.h>
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

    void createBoxForElement(std::shared_ptr<Node> node);
    Glib::RefPtr<Pango::Layout> createTextLayout(const std::shared_ptr<Node> renderNode);
    bool isJustWhiteSpace(std::shared_ptr<Node> node);
private:
    Core* m_core;
    std::shared_ptr<Node> renderElements;
    std::vector<std::shared_ptr<Box>> boxes;
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};;
