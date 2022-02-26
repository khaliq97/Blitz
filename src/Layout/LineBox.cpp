#include <Layout/LineBox.h>
#include <fmt/core.h>
Layout::LineBox::LineBox(const std::string text, double absoluteFontSize, const Cairo::RefPtr<Cairo::Context>& cairoContext)
    : m_text(text)
{
    m_textLayout = Pango::Layout::create(cairoContext);
    m_textLayout->set_text(text);

    Pango::FontDescription font;

    font.set_family("Monospace"); // TODO: CSS font property
    font.set_weight(Pango::WEIGHT_NORMAL); // TODO: CSS font-weight property

    font.set_absolute_size(absoluteFontSize * Pango::SCALE);

    m_textLayout->set_font_description(font);

    // Set content metrics after font has been set

    m_height = m_textLayout->get_logical_extents().get_height() / Pango::SCALE;
    m_width = m_textLayout->get_logical_extents().get_width() / Pango::SCALE;
}

void Layout::LineBox::setText(const std::string value)
{
     m_textLayout->set_text(value);
     m_text = value;
     compute();
}

void Layout::LineBox::compute()
{
    m_height = m_textLayout->get_logical_extents().get_height() / Pango::SCALE;
    m_width = m_textLayout->get_logical_extents().get_width() / Pango::SCALE;
}
