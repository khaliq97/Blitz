#ifndef LINEBOX_H
#define LINEBOX_H
#include <string>
#include <gtkmm.h>
namespace Layout {

struct Position {
 int x, y;
};

class LineBox
{
public:
    const std::string text() { return m_text; }

    double absoluteX() { return m_absoluteX; }
    double absouluteY() { return m_absoluteY; }
    double width() { return m_width; }
    double height() { return m_height; }

    void setAbsoluteX(double value) { m_absoluteX = value; }
    void setAbsoluteY(double value) { m_absoluteY = value; }
    void setWidth(double value) { m_width = value; }

    Glib::RefPtr<Pango::Layout> textLayout() { return m_textLayout; }
    LineBox(const std::string value, double absoluteFontSize, const Cairo::RefPtr<Cairo::Context> &cairoContext);

    double m_absoluteX = 0;
    double m_absoluteY = 0;
    void compute();
    void setText(const std::string);
private:

    Glib::RefPtr<Pango::Layout> m_textLayout;
    std::string m_text;
    double m_width = 0;
    double m_height = 0;

    Position m_position;
};
}
#endif // LINEBOX_H
