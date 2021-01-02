#pragma once

#include <gtkmm.h>
#include <Layout/Box.h>
class HTMLView : public Gtk::DrawingArea
{
public:
    HTMLView();

    struct
    {
        int x;
        int y;
    } drawCoordinates;

private:
    std::unique_ptr<Gtk::Button> m_button;
    std::vector<std::shared_ptr<Box>> boxes;
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};;
