#include <Layout/HTMLView.h>

bool isSizeSet = false;

HTMLView::HTMLView(Core* core, const std::shared_ptr<Node> renderElements) : m_core(core)
{
    this->drawCoordinates = std::make_shared<DrawCoordinates>();
    this->renderElements = renderElements;
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

void HTMLView::createBoxForElement(std::shared_ptr<Node> node, std::shared_ptr<LayoutBox> layoutBox)
{
    if (auto elementNode = std::dynamic_pointer_cast<Element>(node))
    {
        if (elementNode->tagName != "head" && elementNode->tagName != "title")
        {
            if (!root)
            {
                root = std::make_shared<LayoutBox>(this->create_pango_layout(elementNode->getTextContent()), elementNode);

                if (root->element->tagName == "html")
                {
                    root->width = this->get_width();
                    root->borderRect.width = this->get_width();
                }

                layoutBox = root;
            }
            else
            {
                std::shared_ptr<LayoutBox> box;
                if (elementNode->tagName == "body")
                {
                    // FIXME: This is a hack to strip all text from the body as it will contain other elements
                    box = std::make_shared<LayoutBox>(this->create_pango_layout(""), elementNode);
                }
                else
                {
                    box = std::make_shared<LayoutBox>(this->create_pango_layout(elementNode->getTextContent()), elementNode);
                }

                box->parent = layoutBox;
                box->compute();

                if (box->isBlockBox() && box->parent)
                {
                    box->width = box->parent->width - box->parent->borderLeftWidth - box->parent->borderRightWidth - box->parent->paddingLeft - box->parent->paddingRight - box->parent->marginLeft - box->parent->marginRight;
                    box->borderRect.width = box->parent->borderRect.width
                            - box->marginLeft - box->marginRight
                            - box->parent->borderLeftWidth - box->parent->borderRightWidth
                            - box->parent->paddingLeft - box->parent->paddingRight;

                    box->borderRect.y += box->marginTop;
                    box->borderRect.x += box->marginLeft;

                    // First element in the containing block
                    if (layoutBox->children.empty())
                    {
                        box->borderRect.y += box->parent->borderRect.y;

                        // Align box in parent's content view
                        box->borderRect.y += box->parent->paddingTop + box->parent->borderTopWidth;
                        box->borderRect.x += box->parent->borderRect.x + box->parent->paddingLeft + box->parent->borderLeftWidth;
                    }
                    else
                    {
                        // Margin Collapse Logic
                        if (box->parent->children.at(box->parent->children.size() - 1)->marginBottom > box->marginTop ||
                                box->parent->children.at(box->parent->children.size() - 1)->marginBottom == box->marginTop)
                        {
                          box->borderRect.y -= box->marginTop;
                          box->height -= box->marginTop;
                        }

                        // Align box in parent's content view
                        box->borderRect.y += box->parent->paddingTop + box->parent->borderTopWidth;
                        box->borderRect.x += box->parent->borderRect.x + box->parent->paddingLeft + box->parent->borderLeftWidth;

                        // This is the Y position of the last block box in the box's containing box.
                        double containingBlockY = 0;
                        for (int i = 0; i < box->parent->children.size(); i++)
                        {
                            containingBlockY += box->parent->children[i]->height;
                        }

                        box->borderRect.y += box->parent->borderRect.y + containingBlockY;
                    }
                }

                // Increase all of the containing block heights
                box->parent->children.push_back(box);

                box->updateAllParentHeights();

                // Update the box in this function to be the new box
                layoutBox = box;

                this->maxHeight += box->height;
                if (box->borderRect.width > layoutBox->borderRect.width)
                    this->maxWidth += box->borderRect.width;
            }

        }
    }

    for (int i = 0; i < node->childNodes.size(); i++)
    {
        createBoxForElement(node->childNodes[i], layoutBox);
    }
}

void HTMLView::paintLayoutBox(const Cairo::RefPtr<Cairo::Context>& cr, std::string indent, std::shared_ptr<LayoutBox> layoutBox)
{
    printf("%s\033[0;33m<LayoutBox> (%s)\033[0m\n", indent.c_str(), layoutBox->element->tagName.c_str());

    printf("%s    Containing Block: (%s)\n", indent.c_str(), layoutBox->parent ? layoutBox->parent->element->tagName.c_str() : "<Initial Containing Block>");
    printf("%s    Layout Box:\n", indent.c_str());
    printf("%s        Height: %lf\n", indent.c_str(), layoutBox->height);
    printf("%s        Width: %lf\n", indent.c_str(), layoutBox->width);
    printf("%s    Border Rectangle:\n", indent.c_str());
    printf("%s        Height: %lf\n", indent.c_str(), layoutBox->borderRect.height);
    printf("%s        Width: %lf\n", indent.c_str(), layoutBox->borderRect.width);
    printf("%s        Coords (x,y): \n", indent.c_str());
    printf("%s            (%lf, %lf)\n", indent.c_str(), layoutBox->borderRect.x, layoutBox->borderRect.y);


    layoutBox->paint(cr);
    indent += "    ";
    for (int i = 0; i < layoutBox->children.size(); i++)
    {
        paintLayoutBox(cr, indent, layoutBox->children[i]);
    }
}

bool HTMLView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    // Make the background of the view white

    this->set_hexpand(true);
    this->set_halign(Gtk::ALIGN_FILL);
    this->set_vexpand(true);
    this->set_valign(Gtk::ALIGN_FILL);

    cr->set_source_rgb(255, 255, 255);
    cr->rectangle(0, 0, this->get_width(), this->get_height());
    cr->fill();
    cr->stroke();

    root = {};
    createBoxForElement(renderElements, {});

    if (!isSizeSet)
    {
        set_size_request(maxWidth, maxHeight);
        isSizeSet = true;
    }


    printf("------------------------------Layout Tree------------------------------\n");
    paintLayoutBox(cr, "", root);


    return true;
}
