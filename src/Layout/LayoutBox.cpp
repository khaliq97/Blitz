#include <Layout/LayoutBox.h>
#include <Layout/HTMLView.h>
#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm.h>
#include <pangomm.h>
#include <pangomm/init.h>
#include <cairomm/cairomm.h>

bool paintMarginBox = false;

LayoutBox::LayoutBox(const Glib::RefPtr<Pango::Layout>& pangoLayout, const std::shared_ptr<DrawCoordinates>& drawCoords, const std::shared_ptr<Element> &element) : pangoLayout(std::move(pangoLayout)), drawCoords(drawCoords), element(element)
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

    this->borderTopColor = std::move(element->getStylePropertyByDeclarationName("border-top-color")->color);
    this->borderBottomColor = std::move(element->getStylePropertyByDeclarationName("border-bottom-color")->color);
    this->borderLeftColor = std::move(element->getStylePropertyByDeclarationName("border-left-color")->color);
    this->borderRightColor = std::move(element->getStylePropertyByDeclarationName("border-right-color")->color);

    this->fontSize = element->getStylePropertyByDeclarationName("font-size")->computedValue;
}

void LayoutBox::createTextLayout()
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

void LayoutBox::compute()
{
    createTextLayout();

    this->height = this->height + contentHeight + paddingTop + paddingBottom + borderTopWidth + borderBottomWidth + marginTop + marginBottom;
    this->width = this->width + contentWidth + paddingLeft + paddingRight + borderLeftWidth + borderRightWidth + marginLeft + marginRight;
}

void LayoutBox::drawBorder(Border border, Cairo::Rectangle borderRect, const Cairo::RefPtr<Cairo::Context> &cr)
{

    double stepX = 0;
    double stepY = 0;

    localDrawY += marginTop;
    localDrawX += marginLeft;

    switch (border)
    {
        case Border::Top: {

            borderRect.y += marginTop;
            borderRect.x += marginLeft;
            // Left half:
            double leftHalfStep = borderLeftWidth / borderTopWidth;
            cr->move_to(borderRect.x - 1, borderRect.y);


            stepX = borderRect.x;
            for (int i = 0; i < borderTopWidth; i++)
            {
                // Drawing from right to left (starting from the beginning of the border rect)
                cr->line_to(borderRect.x + borderRect.width / 2, localDrawY);
                cr->move_to(stepX, ++localDrawY);
                stepX += leftHalfStep;
            }
            localDrawY -= borderTopWidth;
            cr->set_source_rgb(borderTopColor->r, borderTopColor->g, borderTopColor->b);
            cr->stroke();

            // Right Half:
            double rightHalfStep = borderRightWidth / borderTopWidth;
            cr->move_to(borderRect.x + borderRect.width + 1, borderRect.y);
            stepX = borderRect.x + borderRect.width;
            for (int i = 0; i < borderTopWidth; i++)
            {
                // Drawing from left to right (starting from the max width of the border rect)
                cr->line_to(borderRect.x + borderRect.width / 2, localDrawY);
                cr->move_to(stepX, ++localDrawY);
                stepX -=  rightHalfStep;

            }
            localDrawY -= borderTopWidth;
            cr->set_source_rgb(borderTopColor->r, borderTopColor->g, borderTopColor->b);
            cr->stroke();

            localDrawY -= borderRect.y;


            break;

        }

        case Border::Bottom: {

            // Left half:

            borderRect.x += marginLeft;

            double leftHalfStep = borderLeftWidth / borderBottomWidth;
            cr->move_to(borderRect.x - 1, borderRect.y + borderRect.height + marginTop);
            localDrawY += borderRect.y + borderRect.height + marginTop;

            stepX = borderRect.x;
            for (int i = 0; i < borderBottomWidth; i++)
            {
                // Drawing from right to left (starting from the beginning of the border rect)
                cr->line_to(borderRect.x + borderRect.width / 2, localDrawY);
                cr->move_to(stepX, --localDrawY);
                stepX += leftHalfStep;
            }
            localDrawY += borderBottomWidth;
            cr->set_source_rgb(borderBottomColor->r, borderBottomColor->g, borderBottomColor->b);
            cr->stroke();

            // Right Half:
            double rightHalfStep = borderRightWidth / borderBottomWidth;
            cr->move_to(borderRect.x + borderRect.width + 1, borderRect.y + borderRect.height + marginTop);
            stepX = borderRect.x + borderRect.width;
            for (int i = 0; i < borderBottomWidth; i++)
            {
                // Drawing from left to right (starting from the max width of the border rect)
                cr->line_to(borderRect.x + borderRect.width / 2, localDrawY);
                cr->move_to(stepX, --localDrawY);
                stepX -= rightHalfStep;

            }
            localDrawY += borderBottomWidth;
            cr->set_source_rgb(borderBottomColor->r, borderBottomColor->g, borderBottomColor->b);
            cr->stroke();
            break;
        }

        case Border::Left: {

        borderRect.y += marginTop;
        borderRect.x += marginLeft;

            // Top half:
            double topHalfStep = borderTopWidth / borderLeftWidth;
            cr->move_to(borderRect.x, borderRect.y - 1);

            stepY = borderRect.y;
            for (int i = 0; i < borderLeftWidth; i++)
            {
                // Drawing from right to left (starting from the beginning of the border rect)
                cr->line_to(localDrawX, borderRect.y + borderRect.height / 2);
                cr->move_to(++localDrawX, stepY);
                stepY += topHalfStep;
            }
            localDrawX -= borderLeftWidth;
            cr->set_source_rgb(borderLeftColor->r, borderLeftColor->g, borderLeftColor->b);
            cr->stroke();

            // Bottom Half:
            double bottomHalfStep = borderBottomWidth / borderLeftWidth;
            cr->move_to(borderRect.x, borderRect.y + borderRect.height + 1);
            stepY += borderRect.height - borderTopWidth;
            for (int i = 0; i < borderLeftWidth; i++)
            {
                // Drawing from left to right (starting from the max width of the border rect)
                cr->line_to(localDrawX, borderRect.y + borderRect.height / 2);
                cr->move_to(++localDrawX, stepY);
                stepY -= bottomHalfStep;

            }
            localDrawX -= borderLeftWidth;
            cr->stroke();
            break;
        }

        case Border::Right: {

            borderRect.y += marginTop;
            borderRect.x += marginLeft;

            // Top half:
            double topHalfStep = borderTopWidth / borderRightWidth;
            cr->move_to(borderRect.x + borderRect.width, borderRect.y - 1);
            localDrawX += borderRect.width;

            stepY = borderRect.y;
            for (int i = 0; i < borderRightWidth; i++)
            {
                // Drawing from right to left (starting from the beginning of the border rect)
                cr->line_to(localDrawX, borderRect.y + borderRect.height / 2);
                cr->move_to(--localDrawX, stepY);
                stepY += topHalfStep;
            }
            localDrawX += borderRightWidth;
            cr->set_source_rgb(borderRightColor->r, borderRightColor->g, borderRightColor->b);
            cr->stroke();

            // Bottom Half:
            double bottomHalfStep = borderBottomWidth / borderRightWidth;
            cr->move_to(borderRect.x + borderRect.width, borderRect.y + borderRect.height + 1);
            stepY += borderRect.height - borderTopWidth;
            for (int i = 0; i < borderRightWidth; i++)
            {
                // Drawing from left to right (starting from the max width of the border rect)
                cr->line_to(localDrawX, borderRect.y + borderRect.height / 2);
                cr->move_to(--localDrawX, stepY);
                stepY -= bottomHalfStep;

            }
            localDrawX += borderRightWidth;
            cr->stroke();
            break;
        }



    }

    localDrawY -= marginTop;
    localDrawX -= marginLeft;
}

bool LayoutBox::paint(const Cairo::RefPtr<Cairo::Context> &cr)
{

    borderRect.x = localDrawX;
    borderRect.y = localDrawY;

    // Paint Phase: Margin:
    borderRect.height = borderBottomWidth + paddingBottom + contentHeight + paddingTop + borderTopWidth;
    borderRect.width = borderLeftWidth + paddingLeft + contentWidth + paddingRight + borderRightWidth;
    // Margin drawing - Red
    if (paintMarginBox)
    {
        cr->set_source_rgb(255, 0, 0);
        cr->rectangle(borderRect.x, borderRect.y,  borderRect.width + marginLeft + marginRight,
                      borderRect.height + marginTop + marginBottom);

        // Paint margin
        cr->set_line_width(2);
        cr->stroke();
    }

    // Paint Phase: Border:
    drawBorder(Border::Top, borderRect, cr);
    drawBorder(Border::Bottom, borderRect, cr);
    drawBorder(Border::Left, borderRect, cr);
    drawBorder(Border::Right, borderRect, cr);

    // Paint Phase: Content:
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(borderRect.x + marginLeft + borderLeftWidth + paddingLeft, borderRect.y + marginTop + borderTopWidth + paddingTop);
    pangoLayout->show_in_cairo_context(cr);

    return true;
}

