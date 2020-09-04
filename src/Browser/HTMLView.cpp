#include <Browser/HTMLView.h>
#include <memory>
cairo_surface_t *surface;
cairo_t *cr;

int drawX = 0;
int drawY = 0;

int renderNodeIndex = 0;

bool firstRenderNodeDraw = false;

int defaultFontSize = 16;

HTMLView::HTMLView(std::shared_ptr<Node> renderDocument)
{
    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 600);
    cr = cairo_create(surface);
    this->renderNodes = renderDocument;
}

bool HTMLView::isJustWhiteSpace(std::shared_ptr<Node> node)
{
    bool isJustWhiteSpace = false;
    std::string textContent = node->getTextContent();
    for (int i = 0; i < textContent.length(); i++)
    {
        if (textContent.at(i) == ' ' || textContent.at(i) == '\n' || textContent.at(i) == '\t')
        {
            isJustWhiteSpace = true;
        }
        else
        {
            return false;
        }
    }

    return isJustWhiteSpace;
}

Glib::RefPtr<Pango::Layout> HTMLView::createTextLayout(const Cairo::RefPtr<Cairo::Context> & cr, const std::shared_ptr<Node> renderNode)
{
    auto layout = create_pango_layout(renderNode->getTextContent());

    Pango::FontDescription font;

    font.set_family("Monospace");
    font.set_weight(Pango::WEIGHT_NORMAL);
    font.set_absolute_size(renderNode->layout->fontSize->renderLength * Pango::SCALE);
    layout->set_width(1280 * Pango::SCALE);
        //layout->set_alignment(Pango::Alignment::ALIGN_LEFT);
    layout->set_wrap(Pango::WrapMode::WRAP_WORD);

    layout->set_font_description(font);

    return layout;
}

void HTMLView::processRenderStyle(std::shared_ptr<Node> node)
{
    if (node->css.get() != nullptr)
    {
        if (node->css->styleRules->styleMap->size() > 0)
        {
            node->layout = std::make_shared<Layout>();
            for (auto stylePropVal: *node->css->styleRules->styleMap)
            {
                if (stylePropVal.first.compare("font-size") == 0)
                {
                    node->layout->fontSize->value = parseLengthValue(stylePropVal.second);
                    node->layout->fontSize->lengthUnit = parseLengthUnit(stylePropVal.second);

                    if (node->parentNode.get() != nullptr && node->parentNode->layout.get() != nullptr && node->parentNode->layout->fontSize.get() != nullptr)
                        node->layout->fontSize->setPangoScaleFromLengthUnit(node->parentNode->layout->fontSize->value, node->layout->fontSize->value);
                }
                else if (stylePropVal.first.compare("display") == 0)
                {
                    node->layout->display = stylePropVal.second;
                }
                else if (stylePropVal.first.compare("padding-top") == 0)
                {
                    node->layout->paddingTop->value = parseLengthValue(stylePropVal.second);
                    node->layout->paddingTop->lengthUnit = parseLengthUnit(stylePropVal.second);

                    if (node->parentNode.get() != nullptr && node->parentNode->layout.get() != nullptr && node->parentNode->layout->fontSize.get() != nullptr)
                        node->layout->paddingTop->setPangoScaleFromLengthUnit(node->parentNode->layout->fontSize->value, node->layout->paddingTop->value);
                }
                else if (stylePropVal.first.compare("padding-bottom") == 0)
                {
                    node->layout->paddingBottom->value = parseLengthValue(stylePropVal.second);
                    node->layout->paddingBottom->lengthUnit = parseLengthUnit(stylePropVal.second);

                    if (node->parentNode.get() != nullptr && node->parentNode->layout.get() != nullptr && node->parentNode->layout->fontSize.get() != nullptr)
                        node->layout->paddingBottom->setPangoScaleFromLengthUnit(node->parentNode->layout->fontSize->value, node->layout->fontSize->value);
                }
                else if (stylePropVal.first.compare("padding-left") == 0)
                {
                    node->layout->paddingLeft->value = parseLengthValue(stylePropVal.second);
                    node->layout->paddingLeft->lengthUnit = parseLengthUnit(stylePropVal.second);

                    if (node->parentNode.get() != nullptr && node->parentNode->layout.get() != nullptr && node->parentNode->layout->fontSize.get() != nullptr)
                    {
                        node->layout->paddingLeft->setPangoScaleFromLengthUnit(node->parentNode->layout->fontSize->value, node->layout->paddingLeft->value);
                    }
                }
                else if (stylePropVal.first.compare("padding-right") == 0)
                {
                    node->layout->paddingRight->value = parseLengthValue(stylePropVal.second);
                    node->layout->paddingRight->lengthUnit = parseLengthUnit(stylePropVal.second);

                    if (node->parentNode.get() != nullptr && node->parentNode->layout.get() != nullptr && node->parentNode->layout->fontSize.get() != nullptr)
                        node->layout->paddingRight->setPangoScaleFromLengthUnit(node->parentNode->layout->fontSize->value, node->layout->fontSize->value);
                }
                else if (stylePropVal.first.compare("margin-top") == 0)
                {
                    node->layout->marginTop->value = parseLengthValue(stylePropVal.second);
                    node->layout->marginTop->lengthUnit = parseLengthUnit(stylePropVal.second);

                    if (node->parentNode.get() != nullptr && node->parentNode->layout.get() != nullptr && node->parentNode->layout->fontSize.get() != nullptr)
                        node->layout->marginTop->setPangoScaleFromLengthUnit(node->parentNode->layout->fontSize->value, node->layout->fontSize->value);
                }
                else if (stylePropVal.first.compare("margin-bottom") == 0)
                {
                    node->layout->marginBottom->value = parseLengthValue(stylePropVal.second);
                    node->layout->marginBottom->lengthUnit = parseLengthUnit(stylePropVal.second);

                    if (node->parentNode.get() != nullptr && node->parentNode->layout.get() != nullptr && node->parentNode->layout->fontSize.get() != nullptr)
                        node->layout->marginBottom->setPangoScaleFromLengthUnit(node->parentNode->layout->fontSize->value, node->layout->fontSize->value);
                }
                else if (stylePropVal.first.compare("margin-left") == 0)
                {
                    node->layout->marginLeft->value = parseLengthValue(stylePropVal.second);
                    node->layout->marginLeft->lengthUnit = parseLengthUnit(stylePropVal.second);

                    if (node->parentNode.get() != nullptr && node->parentNode->layout.get() != nullptr && node->parentNode->layout->fontSize.get() != nullptr)
                        node->layout->marginLeft->setPangoScaleFromLengthUnit(node->parentNode->layout->fontSize->value, node->layout->fontSize->value);
                }
                else if (stylePropVal.first.compare("margin-right") == 0)
                {
                    node->layout->marginRight->value = parseLengthValue(stylePropVal.second);
                    node->layout->marginRight->lengthUnit = parseLengthUnit(stylePropVal.second);

                    if (node->parentNode.get() != nullptr && node->parentNode->layout.get() != nullptr && node->parentNode->layout->fontSize.get() != nullptr)
                        node->layout->marginRight->setPangoScaleFromLengthUnit(node->parentNode->layout->fontSize->value, node->layout->fontSize->value);

                }else if (stylePropVal.first.compare("border") == 0)
                {
                    node->layout->borderThickness->value = parseLengthValue(stylePropVal.second);
                    node->layout->borderThickness->lengthUnit = parseLengthUnit(stylePropVal.second);
                }
                else
                {
                    printf("\033[1;33mRender Tree Warning:\033[0m %s css property not recognized\n", stylePropVal.first.c_str());
                }

            }
        }
        //printf("Padding Top: %f\n", node->layout->paddingTop->value);
        //printf("Margin Top: %f\n", node->layout->marginTop->value);
        //printf("Margin Top Unit: %s\n", node->layout->marginTop->lengthUnit.c_str());
    }

    for (int i = 0; i < node->childNodes.size(); i++)
    {
        processRenderStyle(node->childNodes[i]);
    }
}

void HTMLView::processRenderTree()
{
    processRenderStyle(renderNodes);
}

double HTMLView::parseLengthValue(std::string styleValue)
{
    std::string lengthValue;

    double returnLengthValue;
    for (int i = 0; i < styleValue.length(); i++)
    {
        if (std::isdigit(styleValue.at(i)) || styleValue.at(i) == '.')
        {
            lengthValue+= styleValue.at(i);
        }
    }

    returnLengthValue = atof(lengthValue.c_str());

    return returnLengthValue;
}

std::string HTMLView::parseLengthUnit(std::string styleValue)
{
    std::string returnLengthUnit;

    for (int i = 0; i < styleValue.length(); i++)
    {
        if (std::isalpha(styleValue.at(i)))
        {
            returnLengthUnit+= styleValue.at(i);
        }
    }

    return returnLengthUnit;
}

double calculateEM(double fontSize, double em)
{
    return fontSize * em;
}



void HTMLView::drawBorder(Cairo::RefPtr<Cairo::Context> const & cr, std::shared_ptr<Node> renderNode)
{

}

void HTMLView::drawMargin(Cairo::RefPtr<Cairo::Context> const & cr, std::shared_ptr<Node> renderNode)
{

}

void HTMLView::drawRenderNode(Cairo::RefPtr<Cairo::Context> const & cr, std::shared_ptr<Node> node)
{

    if (auto elementNode = dynamic_cast<HTMLElement*>(node.get()))
    {
        if (node->css.get() != nullptr && node->layout.get() != nullptr && !isJustWhiteSpace(node))
        {
            auto nodeLayout = createTextLayout(cr, node);
            //layouts.push_back(nodeLayout);
            Pango::Rectangle inkNodeRectange;
            Pango::Rectangle logicalNodeRectange;
            nodeLayout->get_extents(inkNodeRectange, logicalNodeRectange);

            int nodeLayoutHeight = logicalNodeRectange.get_height() / Pango::SCALE;
            int nodeLayoutWidth = logicalNodeRectange.get_width() / Pango::SCALE;

            printf("Draw Margin X: %d, Draw Margin Y: %d\n", drawX, drawY);

            if (node->layout->display.compare("block") == 0)
            {
                if (drawnRenderNodes.size() > 0)
                {
                    printf("Rendered Height: %f\n", drawnRenderNodes[drawnRenderNodes.size() - 1]->layout->renderedHeight);
                    drawY += drawnRenderNodes[drawnRenderNodes.size() - 1]->layout->renderedHeight;
                }
            }
            else if (node->layout->display.compare("inline") == 0)
            {
                if (drawnRenderNodes.size() > 0)
                {
                    printf("Rendered Width: %f\n", drawnRenderNodes[drawnRenderNodes.size() - 1]->layout->renderedHeight);
                    drawX += drawnRenderNodes[drawnRenderNodes.size() - 1]->layout->renderedWidth;
                }

            }

            // Draw Margin
            cr->set_source_rgb(0, 180, 0.0);
            cr->rectangle(drawX, drawY, nodeLayoutWidth + node->layout->paddingLeft->renderLength + node->layout->paddingRight->renderLength
                          + node->layout->marginLeft->renderLength + node->layout->marginRight->renderLength,
                          nodeLayoutHeight + node->layout->paddingTop->renderLength + node->layout->paddingBottom->renderLength +
                          node->layout->marginTop->renderLength + node->layout->marginBottom->renderLength);

            cr->stroke();

            printf("Draw Padding X: %d, Draw Padding Y: %d\n", drawX, drawY);

            // Draw Padding
            cr->set_source_rgb(0.8, 0.0, 0.0); // TODO: Should be set by css property
            printf("Padding Render Left: %f\n", node->layout->paddingLeft->renderLength);
            cr->rectangle(drawX + node->layout->marginLeft->renderLength, drawY + node->layout->marginTop->renderLength, nodeLayoutWidth + node->layout->paddingLeft->renderLength +  node->layout->paddingRight->renderLength,
                          nodeLayoutHeight + node->layout->paddingTop->renderLength + node->layout->paddingBottom->renderLength);
            cr->stroke();

            //drawX += node->layout->paddingLeft->renderLength;

            cr->set_source_rgb(0.0, 0.0, 0.0);
            cr->move_to(drawX + node->layout->paddingLeft->renderLength, drawY + node->layout->marginTop->renderLength + node->layout->paddingTop->renderLength);
            nodeLayout->show_in_cairo_context(cr);

            node->layout->renderedHeight = nodeLayoutHeight + node->layout->paddingTop->renderLength + node->layout->paddingBottom->renderLength +
                    node->layout->marginTop->renderLength + node->layout->marginBottom->renderLength;

            node->layout->renderedWidth = nodeLayoutWidth + node->layout->paddingLeft->renderLength + node->layout->paddingRight->renderLength +
                    node->layout->marginLeft->renderLength + node->layout->marginRight->renderLength;

            drawnRenderNodes.push_back(node);
        }
    }



    for (int i = 0; i < node->childNodes.size(); i++)
    {
        drawRenderNode(cr, node->childNodes[i]);
    }
}

bool HTMLView::on_draw(Cairo::RefPtr<Cairo::Context> const & cr)
{
    //cr->set_source_rgb(0.0, 0.0, 0.0);
    drawX = 0;
    drawY = 0;
    firstRenderNodeDraw = false;
    Pango::Rectangle empty;
    drawnRenderNodes.clear();
    drawRenderNode(cr, renderNodes);


    return true;
}

