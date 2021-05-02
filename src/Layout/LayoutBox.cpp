#include <Layout/LayoutBox.h>
#include <Layout/HTMLView.h>
#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm.h>
#include <pangomm.h>
#include <pangomm/init.h>
#include <cairomm/cairomm.h>

bool paintMarginBox = false;

LayoutBox::LayoutBox(const Glib::RefPtr<Pango::Layout>& pangoLayout, std::shared_ptr<Element>element) : pangoLayout(std::move(pangoLayout)), element(element)
{

    // TODO: Move all of these property setters to a box model class
    this->paddingTop = element->getStylePropertyByDeclarationName("padding-top")->computedValue;
    this->paddingBottom = element->getStylePropertyByDeclarationName("padding-bottom")->computedValue;
    this->paddingLeft = element->getStylePropertyByDeclarationName("padding-left")->computedValue;
    this->paddingRight = element->getStylePropertyByDeclarationName("padding-right")->computedValue;

    this->marginTop = element->getStylePropertyByDeclarationName("margin-block-start")->computedValue;
    this->marginBottom = element->getStylePropertyByDeclarationName("margin-block-end")->computedValue;
    this->marginLeft = element->getStylePropertyByDeclarationName("margin-inline-start")->computedValue;
    this->marginRight = element->getStylePropertyByDeclarationName("margin-inline-end")->computedValue;

    if (element->doesDeclarationExist("margin-top"))
        this->marginTop = element->getStylePropertyByDeclarationName("margin-top")->computedValue;

    if (element->doesDeclarationExist("margin-bottom"))
        this->marginBottom = element->getStylePropertyByDeclarationName("margin-bottom")->computedValue;

    if (element->doesDeclarationExist("margin-left"))
        this->marginLeft = element->getStylePropertyByDeclarationName("margin-left")->computedValue;

    if (element->doesDeclarationExist("margin-right"))
        this->marginRight = element->getStylePropertyByDeclarationName("margin-right")->computedValue;

    this->borderTopWidth = element->getStylePropertyByDeclarationName("border-top-width")->computedValue;
    this->borderBottomWidth = element->getStylePropertyByDeclarationName("border-bottom-width")->computedValue;
    this->borderLeftWidth = element->getStylePropertyByDeclarationName("border-left-width")->computedValue;
    this->borderRightWidth = element->getStylePropertyByDeclarationName("border-right-width")->computedValue;

    this->borderTopColor = element->getStylePropertyByDeclarationName("border-top-color")->color;
    this->borderBottomColor = element->getStylePropertyByDeclarationName("border-bottom-color")->color;
    this->borderLeftColor = element->getStylePropertyByDeclarationName("border-left-color")->color;
    this->borderRightColor = element->getStylePropertyByDeclarationName("border-right-color")->color;

    this->fontSize = element->getStylePropertyByDeclarationName("font-size")->computedValue;

    this->textColor = element->getStylePropertyByDeclarationName("color")->color;

    if (element->doesDeclarationExist("background-color"))
    {
        this->backgroundColor = element->getStylePropertyByDeclarationName("background-color")->color;
    }

    borderRect.x = 0;
    borderRect.y = 0;
}

void LayoutBox::createTextLayout()
{
    Pango::FontDescription font;

    font.set_family("Monospace"); // TODO: CSS font property
    font.set_weight(Pango::WEIGHT_NORMAL); // TODO: CSS font-weight property

    font.set_absolute_size(this->fontSize * Pango::SCALE);

    pangoLayout->set_font_description(font);

    // Set content metrics after font has been set

    contentRect.height = pangoLayout->get_logical_extents().get_height() / Pango::SCALE;
    contentRect.width = pangoLayout->get_logical_extents().get_width() / Pango::SCALE;
}

bool LayoutBox::isBlockBox()
{
    if (element->getStylePropertyByDeclarationName("display"))
        return element->getStylePropertyByDeclarationName("display")->m_declaration->value[0].value() == "block";

    return false;
}

bool LayoutBox::isInlineBox()
{
    if (element->getStylePropertyByDeclarationName("display"))
        return element->getStylePropertyByDeclarationName("display")->m_declaration->value[0].value() == "inline";

    return false;
}

void LayoutBox::appendChildAndUpdateParentHeights(std::shared_ptr<LayoutBox> layoutBox)
{
    parent.lock()->children.push_back(std::move(layoutBox));
    updateAllParentHeights();
}


void LayoutBox::updateAllParentHeights()
{
    if (parent.lock())
    {
        double allContainingLayoutBoxHeight = 0;
        for (int i = 0; i < parent.lock()->children.size(); i++)
        {
            allContainingLayoutBoxHeight += parent.lock()->children[i]->marginRect.height;
        }

        parent.lock()->borderRect.height = parent.lock()->borderTopWidth + parent.lock()->paddingTop + allContainingLayoutBoxHeight + parent.lock()->borderBottomWidth + parent.lock()->paddingBottom;

        parent.lock()->marginRect.height = parent.lock()->marginTop + parent.lock()->borderBottomWidth + parent.lock()->paddingTop + allContainingLayoutBoxHeight + parent.lock()->borderBottomWidth + parent.lock()->paddingBottom + parent.lock()->marginBottom;

        parent.lock()->updateAllParentHeights();
    }
    // TODO: Need a IF condition for the root layout box (i.e. html tag)
}

void LayoutBox::compute()
{
    createTextLayout();

    marginRect.height = marginTop + borderTopWidth + paddingTop + contentRect.height + paddingBottom  + borderBottomWidth + marginBottom;
    marginRect.width = marginLeft + borderLeftWidth + paddingLeft + contentRect.width + paddingRight + borderRightWidth + marginRight;

    borderRect.height = borderTopWidth + paddingTop + contentRect.height + paddingBottom + borderBottomWidth;
    borderRect.width = borderLeftWidth + paddingLeft + contentRect.width + paddingRight + borderRightWidth;
}

void LayoutBox::drawBorder(Border border, Cairo::Rectangle borderRect, const Cairo::RefPtr<Cairo::Context> &cr)
{
    double stepX = 0;
    double stepY = 0;

    switch (border)
    {
        case Border::Top: {

            // Left half:
            double leftHalfStep = borderLeftWidth / borderTopWidth;
            cr->move_to(borderRect.x, borderRect.y);

            stepX = borderRect.x;
            for (int i = 0; i < borderTopWidth; i++)
            {
                // Drawing from right to left (starting from the beginning of the border rect)
                cr->line_to(borderRect.x + borderRect.width / 2, layoutBoxDrawCursor_Y);
                cr->move_to(stepX, ++layoutBoxDrawCursor_Y);
                stepX += leftHalfStep;
            }
            layoutBoxDrawCursor_Y -= borderTopWidth;
            cr->set_source_rgb(borderTopColor->displayColor.get_red(), borderTopColor->displayColor.get_green(), borderTopColor->displayColor.get_blue());
            cr->stroke();

            // Right Half:
            double rightHalfStep = borderRightWidth / borderTopWidth;
            cr->move_to(borderRect.x + borderRect.width, borderRect.y);
            stepX = borderRect.x + borderRect.width;
            for (int i = 0; i < borderTopWidth; i++)
            {
                // Drawing from left to right (starting from the max width of the border rect)
                cr->line_to(borderRect.x + borderRect.width / 2, layoutBoxDrawCursor_Y);
                cr->move_to(stepX, ++layoutBoxDrawCursor_Y);
                stepX -=  rightHalfStep;

            }
            layoutBoxDrawCursor_Y -= borderTopWidth;
            cr->stroke();

            layoutBoxDrawCursor_Y -= borderRect.y;
            break;

        }

        case Border::Bottom: {

            // Left half:

            double leftHalfStep = borderLeftWidth / borderBottomWidth;
            cr->move_to(borderRect.x, borderRect.y + borderRect.height);
            layoutBoxDrawCursor_Y += borderRect.y + borderRect.height;

            stepX = borderRect.x;
            for (int i = 0; i < borderBottomWidth; i++)
            {
                // Drawing from right to left (starting from the beginning of the border rect)
                cr->line_to(borderRect.x + borderRect.width / 2, layoutBoxDrawCursor_Y);
                cr->move_to(stepX, --layoutBoxDrawCursor_Y);
                stepX += leftHalfStep;
            }
            layoutBoxDrawCursor_Y += borderBottomWidth;
            cr->set_source_rgb(borderBottomColor->displayColor.get_red(), borderBottomColor->displayColor.get_green(), borderBottomColor->displayColor.get_blue());
            cr->stroke();

            // Right Half:
            double rightHalfStep = borderRightWidth / borderBottomWidth;
            cr->move_to(borderRect.x + borderRect.width, borderRect.y + borderRect.height);
            stepX = borderRect.x + borderRect.width;
            for (int i = 0; i < borderBottomWidth; i++)
            {
                // Drawing from left to right (starting from the max width of the border rect)
                cr->line_to(borderRect.x + borderRect.width / 2, layoutBoxDrawCursor_Y);
                cr->move_to(stepX, --layoutBoxDrawCursor_Y);
                stepX -= rightHalfStep;

            }
            layoutBoxDrawCursor_Y += borderBottomWidth;
            cr->stroke();
            break;
        }

        case Border::Left: {

            // Top half:
            double topHalfStep = borderTopWidth / borderLeftWidth;
            cr->move_to(borderRect.x, borderRect.y);

            stepY = borderRect.y;
            for (int i = 0; i < borderLeftWidth; i++)
            {
                // Drawing from right to left (starting from the beginning of the border rect)
                cr->line_to(layoutBoxDrawCursor_X, borderRect.y + borderRect.height / 2);
                cr->move_to(++layoutBoxDrawCursor_X, stepY);
                stepY += topHalfStep;
            }
            layoutBoxDrawCursor_X -= borderLeftWidth;
            cr->set_source_rgb(borderLeftColor->displayColor.get_red(), borderLeftColor->displayColor.get_green(), borderLeftColor->displayColor.get_blue());
            cr->stroke();

            // Bottom Half:
            double bottomHalfStep = borderBottomWidth / borderLeftWidth;
            cr->move_to(borderRect.x, borderRect.y + borderRect.height);
            stepY += borderRect.height - borderTopWidth;
            for (int i = 0; i < borderLeftWidth; i++)
            {
                // Drawing from left to right (starting from the max width of the border rect)
                cr->line_to(layoutBoxDrawCursor_X, borderRect.y + borderRect.height / 2);
                cr->move_to(++layoutBoxDrawCursor_X, stepY);
                stepY -= bottomHalfStep;

            }
            layoutBoxDrawCursor_X -= borderLeftWidth;
            cr->stroke();
            break;
        }

        case Border::Right: {

            // Top half:
            double topHalfStep = borderTopWidth / borderRightWidth;
            cr->move_to(borderRect.x + borderRect.width, borderRect.y);
            layoutBoxDrawCursor_X += borderRect.width;

            stepY = borderRect.y;
            for (int i = 0; i < borderRightWidth; i++)
            {
                // Drawing from right to left (starting from the beginning of the border rect)
                cr->line_to(layoutBoxDrawCursor_X, borderRect.y + borderRect.height / 2);
                cr->move_to(--layoutBoxDrawCursor_X, stepY);
                stepY += topHalfStep;
            }
            layoutBoxDrawCursor_X += borderRightWidth;
            cr->set_source_rgb(borderRightColor->displayColor.get_red(), borderRightColor->displayColor.get_green(), borderRightColor->displayColor.get_blue());
            cr->stroke();

            // Bottom Half:
            double bottomHalfStep = borderBottomWidth / borderRightWidth;
            cr->move_to(borderRect.x + borderRect.width, borderRect.y + borderRect.height);
            stepY += borderRect.height - borderTopWidth;
            for (int i = 0; i < borderRightWidth; i++)
            {
                // Drawing from left to right (starting from the max width of the border rect)
                cr->line_to(layoutBoxDrawCursor_X, borderRect.y + borderRect.height / 2);
                cr->move_to(--layoutBoxDrawCursor_X, stepY);
                stepY -= bottomHalfStep;

            }
            layoutBoxDrawCursor_X += borderRightWidth;
            cr->stroke();
            break;
        }

    }
}

bool LayoutBox::paint(const Cairo::RefPtr<Cairo::Context> &cr)
{
    // Paint Phase: Margin:
    // Margin drawing - Red
    if (paintMarginBox)
    {
        cr->set_source_rgb(255, 0, 0);
        cr->rectangle(borderRect.x - marginLeft, borderRect.y - marginTop, marginRect.width,
                      marginRect.height);

        // Paint margin
        cr->set_line_width(2);
        cr->stroke();
    }


    // Update local draw cursor
    layoutBoxDrawCursor_X = borderRect.x;
    layoutBoxDrawCursor_Y = borderRect.y;

    // Paint Phase: Border:
    drawBorder(Border::Top, borderRect, cr);
    drawBorder(Border::Bottom, borderRect, cr);
    drawBorder(Border::Left, borderRect, cr);
    drawBorder(Border::Right, borderRect, cr);

    // Paint Phase: Background Color:
    if (backgroundColor)
    {
        cr->set_source_rgb(backgroundColor->displayColor.get_red(), backgroundColor->displayColor.get_green(), backgroundColor->displayColor.get_blue());
        cr->rectangle(borderRect.x + borderLeftWidth, borderRect.y + borderTopWidth, borderRect.width - borderLeftWidth - borderRightWidth,
                      borderRect.height - borderTopWidth - borderBottomWidth);

        // Paint margin
        cr->stroke_preserve();
        cr->fill();
    }

    // Update position of content in border rectangle.
    contentRect.y = borderRect.y + borderTopWidth + paddingTop;
    contentRect.x = borderRect.x + borderLeftWidth + paddingLeft;


    // TODO: This is a very primitive way to render text, these should exist as LineBox objects and be
    // drawn how boxes are drawn in HTMLView (i.e. a box within a box and not text).

    // Paint Phase: Content:
    cr->set_source_rgb(textColor->displayColor.get_red(), textColor->displayColor.get_green(), textColor->displayColor.get_blue());
    cr->move_to(contentRect.x, contentRect.y);
    pangoLayout->show_in_cairo_context(cr);

    // Reset back to white
    cr->set_source_rgb(0.0, 0.0, 0.0);


    return true;
}

