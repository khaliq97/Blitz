#include <Layout/LayoutBox.h>
#include <Layout/HTMLView.h>
#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm.h>
#include <pangomm.h>
#include <pangomm/init.h>
#include <cairomm/cairomm.h>
#include <fmt/core.h>
bool paintMarginBox = false;

LayoutBox::LayoutBox(std::shared_ptr<Node> element, BlockType type) : m_type(type)
{
    if (element->isElement())
        this->m_element = element->asNodeTypeElement(element).lock();

    // TODO: Move all of these property setters to a box model class
    this->paddingTop = m_element->getStylePropertyByDeclarationName("padding-top")->computedValue;
    this->paddingBottom = m_element->getStylePropertyByDeclarationName("padding-bottom")->computedValue;
    this->paddingLeft = m_element->getStylePropertyByDeclarationName("padding-left")->computedValue;
    this->paddingRight = m_element->getStylePropertyByDeclarationName("padding-right")->computedValue;

    this->marginTop = m_element->getStylePropertyByDeclarationName("margin-block-start")->computedValue;
    this->marginBottom = m_element->getStylePropertyByDeclarationName("margin-block-end")->computedValue;
    this->marginLeft = m_element->getStylePropertyByDeclarationName("margin-inline-start")->computedValue;
    this->marginRight = m_element->getStylePropertyByDeclarationName("margin-inline-end")->computedValue;

    if (m_element->doesDeclarationExist("margin-top"))
        this->marginTop = m_element->getStylePropertyByDeclarationName("margin-top")->computedValue;

    if (m_element->doesDeclarationExist("margin-bottom"))
        this->marginBottom = m_element->getStylePropertyByDeclarationName("margin-bottom")->computedValue;

    if (m_element->doesDeclarationExist("margin-left"))
        this->marginLeft = m_element->getStylePropertyByDeclarationName("margin-left")->computedValue;

    if (m_element->doesDeclarationExist("margin-right"))
        this->marginRight = m_element->getStylePropertyByDeclarationName("margin-right")->computedValue;

    this->borderTopWidth = m_element->getStylePropertyByDeclarationName("border-top-width")->computedValue;
    this->borderBottomWidth = m_element->getStylePropertyByDeclarationName("border-bottom-width")->computedValue;
    this->borderLeftWidth = m_element->getStylePropertyByDeclarationName("border-left-width")->computedValue;
    this->borderRightWidth = m_element->getStylePropertyByDeclarationName("border-right-width")->computedValue;

    this->borderTopColor = m_element->getStylePropertyByDeclarationName("border-top-color")->color;
    this->borderBottomColor = m_element->getStylePropertyByDeclarationName("border-bottom-color")->color;
    this->borderLeftColor = m_element->getStylePropertyByDeclarationName("border-left-color")->color;
    this->borderRightColor = m_element->getStylePropertyByDeclarationName("border-right-color")->color;

    this->fontSize = m_element->getStylePropertyByDeclarationName("font-size")->computedValue;

    this->textColor = m_element->getStylePropertyByDeclarationName("color")->color;

    if (m_element->doesDeclarationExist("background-color"))
    {
        this->backgroundColor = m_element->getStylePropertyByDeclarationName("background-color")->color;
    }

    borderRect.x = 0;
    borderRect.y = 0;

    marginRect.x = 0;
    marginRect.y = 0;

    contentRect.x = 0;
    contentRect.y = 0;

    marginRect.height = marginTop + borderTopWidth + paddingTop + contentRect.height + paddingBottom  + borderBottomWidth + marginBottom;
    marginRect.width = marginLeft + borderLeftWidth + paddingLeft + contentRect.width + paddingRight + borderRightWidth + marginRight;

    borderRect.height = borderTopWidth + paddingTop + contentRect.height + paddingBottom + borderBottomWidth;
    borderRect.width = borderLeftWidth + paddingLeft + contentRect.width + paddingRight + borderRightWidth;
}

// FIXME: This is quite buggy when individual line boxes start to only contain 1 or 2 words. Need to investigate why this happens.
void LayoutBox::updateLineBoxes(const Cairo::RefPtr<Cairo::Context>& cairoContext, int index)
{
    std::shared_ptr<Layout::LineBox> lineBoxCurrent = m_lineBoxes[index];

    if (index >= 1)
    {
        std::shared_ptr<Layout::LineBox> lineBoxBeforeCurrent = m_lineBoxes[index - 1];
        if (lineBoxBeforeCurrent->width() > borderRect.width)
        {

             Layout::LayoutText lineBoxTextBeforeCurrent = Layout::LayoutText(lineBoxBeforeCurrent->text());
             Layout::LayoutText lineBoxTextCurrent = Layout::LayoutText(lineBoxCurrent->text());

             lineBoxTextCurrent.appendWordToBeginning(lineBoxTextBeforeCurrent.removeLastWord());

             lineBoxBeforeCurrent->setText(lineBoxTextBeforeCurrent.displayText());
             lineBoxCurrent->setText(lineBoxTextCurrent.displayText());

             updateLineBoxes(cairoContext, index);
        }

    }
    if (lineBoxCurrent->width() > borderRect.width )
    {
        Layout::LayoutText beforeUpdateText = Layout::LayoutText(lineBoxCurrent->text());
        std::string lastWord = beforeUpdateText.removeLastWord();
        lineBoxCurrent->setText(beforeUpdateText.displayText());

        std::shared_ptr<Layout::LineBox> newLineBox = std::make_shared<Layout::LineBox>(lastWord, this->fontSize, cairoContext);
        m_lineBoxes.push_back(newLineBox);
        index++;
        updateLineBoxes(cairoContext, index);

    }
}

void LayoutBox::createLineBoxes(const Cairo::RefPtr<Cairo::Context>& cairoContext)
{
    std::vector<Layout::LineBox> lineBoxes;
    for (auto node: this->m_element->childNodes)
    {
        if (node->isText())
        {
            auto textNode = node->asNodeTypeText(node);

            if (!Tools::isJustWhiteSpace(textNode.lock()->data))
            {
                auto initialLineBox = std::make_shared<Layout::LineBox>(textNode.lock()->data, this->fontSize, cairoContext);
                m_lineBoxes.push_back(initialLineBox);
                updateLineBoxes(cairoContext, 0);
            }
        }
    }
}

void LayoutBox::createTextLayout(const Cairo::RefPtr<Cairo::Context>& cairoContext)
{
    m_pangoLayout = Pango::Layout::create(cairoContext);

    if (m_element->tagName() == "body")
    {
          m_pangoLayout->set_text("");
    }
    else
    {
          m_pangoLayout->set_text(m_element->getTextContent());
    }


    Pango::FontDescription font;

    font.set_family("Monospace"); // TODO: CSS font property
    font.set_weight(Pango::WEIGHT_NORMAL); // TODO: CSS font-weight property

    font.set_absolute_size(this->fontSize * Pango::SCALE);

    m_pangoLayout->set_font_description(font);

    contentRect.height = m_pangoLayout->get_logical_extents().get_height() / Pango::SCALE;
    contentRect.width = m_pangoLayout->get_logical_extents().get_width() / Pango::SCALE;

    marginRect.height = marginTop + borderTopWidth + paddingTop + contentRect.height + paddingBottom  + borderBottomWidth + marginBottom;
    marginRect.width = marginLeft + borderLeftWidth + paddingLeft + contentRect.width + paddingRight + borderRightWidth + marginRight;

    borderRect.height = borderTopWidth + paddingTop + contentRect.height + paddingBottom + borderBottomWidth;
    borderRect.width = borderLeftWidth + paddingLeft + contentRect.width + paddingRight + borderRightWidth;
}

bool LayoutBox::isBlockBox()
{
    return m_type == BlockType::Block;
}

bool LayoutBox::isInlineBox()
{
  return m_type == BlockType::Inline;
}


void LayoutBox::appendChildAndUpdateParentHeights(std::shared_ptr<LayoutBox> layoutBox)
{
    //parent.lock()->children.push_back(layoutBox);
    updateAllParentHeights();
}

int LayoutBox::calculateYPositionInContainingBlock(std::shared_ptr<LayoutBox>& currentLayoutBox)
{
    int y = 0;
    for (auto layoutBox: parent.lock()->children)
    {
        if (layoutBox == currentLayoutBox)
            break;
        y += layoutBox->marginRect.height;
    }

    return y;
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
}

bool LayoutBox::isFirstLayoutBoxInContainingBlock(const std::shared_ptr<LayoutBox>& layoutBox)
{
    return parent.lock()->children.front() == layoutBox;
}

std::vector<std::shared_ptr<LayoutBox>> LayoutBox::toVectorFromLayoutTree(std::shared_ptr<LayoutBox> initialContainingBlockBox, std::vector<std::shared_ptr<LayoutBox>> layoutVector)
{
    layoutVector.push_back(initialContainingBlockBox);
    for (int i = 0; i < initialContainingBlockBox->children.size(); i++)
    {
        layoutVector = toVectorFromLayoutTree(initialContainingBlockBox->children[i], layoutVector);
    }

    return layoutVector;
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

    // Update position of content in border rectangle.
    contentRect.y = borderRect.y + borderTopWidth + paddingTop;
    contentRect.x = borderRect.x + borderLeftWidth + paddingLeft;

    createLineBoxes(cr);

    int index = 0;
    for (int i = 0; i < m_lineBoxes.size(); i++)
    {
        // fmt::print("LineBox frag ({}), Height: {}: {}\n", index, m_lineBoxes[i]->height(), m_lineBoxes[i]->text());
        cr->set_source_rgb(textColor->displayColor.get_red(), textColor->displayColor.get_green(), textColor->displayColor.get_blue());
        cr->move_to(contentRect.x, contentRect.y);

        if (i != m_lineBoxes.size() - 1)
        {
            borderRect.height += m_lineBoxes[i + 1]->height();
            marginRect.height += m_lineBoxes[i + 1]->height();
        }

        contentRect.y += m_lineBoxes[i]->height();
        m_lineBoxes[i]->textLayout()->show_in_cairo_context(cr);
        index++;
    }

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

    // Reset back to white
    cr->set_source_rgb(0.0, 0.0, 0.0);
    return true;
}



