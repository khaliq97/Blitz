#include <Layout/Box.h>
#include <Layout/HTMLView.h>
#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm.h>
#include <pangomm.h>
#include <pangomm/init.h>
#include <cairomm/cairomm.h>

bool paintMarginBox = true;

Box::Box(const Glib::RefPtr<Pango::Layout>& pangoLayout, const std::shared_ptr<DrawCoordinates>& drawCoords, const std::shared_ptr<Element> &element) : pangoLayout(std::move(pangoLayout)), drawCoords(drawCoords), element(element)
{

    this->paddingTop = element->getStylePropertyByDeclarationName("padding-top")->computedValue;
    this->paddingBottom = element->getStylePropertyByDeclarationName("padding-bottom")->computedValue;
    this->paddingLeft = element->getStylePropertyByDeclarationName("padding-left")->computedValue;
    this->paddingRight = element->getStylePropertyByDeclarationName("padding-right")->computedValue;

    this->marginTop = element->getStylePropertyByDeclarationName("margin-top")->computedValue;
    this->marginBottom = element->getStylePropertyByDeclarationName("margin-bottom")->computedValue;
    this->marginLeft = element->getStylePropertyByDeclarationName("margin-left")->computedValue;
    this->marginRight = element->getStylePropertyByDeclarationName("margin-right")->computedValue;

    this->borderTopWidth = element->getStylePropertyByDeclarationName("border-top-width")->computedValue;
    this->borderBottomWidth = element->getStylePropertyByDeclarationName("border-bottom-width")->computedValue;
    this->borderLeftWidth = element->getStylePropertyByDeclarationName("border-left-width")->computedValue;
    this->borderRightWidth = element->getStylePropertyByDeclarationName("border-right-width")->computedValue;

    this->fontSize = element->getStylePropertyByDeclarationName("font-size")->computedValue;
}

void Box::createTextLayout()
{
    Pango::FontDescription font;

    font.set_family("Monospace"); // TODO: CSS font property
    font.set_weight(Pango::WEIGHT_NORMAL); // TODO: CSS font-weight property
    font.set_absolute_size(this->fontSize * Pango::SCALE);

    pangoLayout->set_font_description(font);

    // Set content metrics after font has been set
    Pango::Rectangle logicalRect = pangoLayout->get_logical_extents();
    this->contentHeight = logicalRect.get_height() / Pango::SCALE;
    this->contentWidth = logicalRect.get_width() / Pango::SCALE;
}

void Box::compute()
{
    createTextLayout();

    this->height = this->height + contentHeight + paddingTop + paddingBottom + borderTopWidth + borderBottomWidth + marginTop + marginBottom;
    this->width = this->width + contentWidth + paddingLeft + paddingRight + borderLeftWidth + borderRightWidth + marginLeft + marginRight;
}


bool Box::paint(const Cairo::RefPtr<Cairo::Context> &cr)
{

    // Margin drawing - Red
    if (paintMarginBox)
    {
        cr->set_source_rgb(255, 0, 0);
        // We add all border-width properties twice as cairo increases the line width from the bottom of the line.
        // So the line's width ends up going upwards and ends up consuming the entire top margin making it useless.
        cr->rectangle(localDrawX, localDrawY, contentWidth + borderLeftWidth + borderRightWidth + paddingLeft + paddingRight + marginLeft + marginRight,
                      contentHeight + borderTopWidth + borderBottomWidth + paddingTop + paddingBottom + marginTop + marginBottom);

        // Paint margin
        cr->set_line_width(2);
        cr->stroke();
    }


    // Border drawing - Blue
    cr->set_source_rgb(0, 0, 255);
    // Left border

    localDrawY += marginTop;
    localDrawX += marginLeft;
    cr->move_to(localDrawX, localDrawY);

    cr->line_to(localDrawX, localDrawY + borderTopWidth + paddingTop + contentHeight + paddingBottom + borderBottomWidth);


    for (int i = 0; i < borderLeftWidth; i++)
    {
        cr->line_to(++localDrawX, localDrawY + borderTopWidth + paddingTop + contentHeight + paddingBottom + borderBottomWidth);
        cr->move_to(localDrawX, localDrawY);
    }

    localDrawX -= borderLeftWidth;
    localDrawY += borderTopWidth + paddingTop + contentHeight + paddingBottom + borderBottomWidth;
    cr->stroke();

    //Bottom Border
    cr->move_to(localDrawX, localDrawY);

    cr->line_to(localDrawX + borderLeftWidth + paddingLeft + contentWidth + paddingRight + borderRightWidth, localDrawY);

    for (int i = 0; i < borderBottomWidth; i++)
    {
        cr->line_to(localDrawX + borderLeftWidth + paddingLeft + contentWidth + paddingRight + borderRightWidth, --localDrawY);
        cr->move_to(localDrawX, localDrawY);

    }
    localDrawY+= borderBottomWidth;

    localDrawX += borderLeftWidth + paddingLeft + contentWidth + paddingRight + borderRightWidth;
    cr->stroke();

    // Right Border
    cr->move_to(localDrawX, localDrawY);

    cr->line_to(localDrawX, localDrawY - borderTopWidth - paddingTop  - contentHeight  - paddingBottom - borderBottomWidth);

    for (int i = 0; i < borderRightWidth; i++)
    {
        cr->line_to(--localDrawX, localDrawY - borderTopWidth - paddingTop  - contentHeight  - paddingBottom - borderBottomWidth);
        cr->move_to(localDrawX, localDrawY);
    }
    localDrawX+= borderRightWidth;
    localDrawY -= borderTopWidth + paddingTop + contentHeight + paddingBottom + borderBottomWidth;
    cr->stroke();

    // Top Border
    cr->move_to(localDrawX, localDrawY);

    cr->line_to(localDrawX - borderLeftWidth - paddingLeft - contentWidth - paddingRight - borderRightWidth, localDrawY);

    for (int i = 0; i < borderTopWidth; i++)
    {
        cr->line_to(localDrawX - borderLeftWidth - paddingLeft - contentWidth - paddingRight - borderRightWidth, ++localDrawY);
        cr->move_to(localDrawX, localDrawY);
    }
    localDrawY-= borderTopWidth;
    localDrawX -= borderLeftWidth + paddingLeft + contentWidth + paddingRight + borderRightWidth;
    cr->stroke();


    // Content drawing (Text only)
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(localDrawX + paddingLeft + borderLeftWidth, localDrawY + paddingTop + borderTopWidth);
    pangoLayout->show_in_cairo_context(cr);

    return true;
}

