#include <Layout/HTMLView.h>

bool isSizeSet = false;

HTMLView::HTMLView(Core* core, const std::shared_ptr<Node> renderElements) : m_core(core)
{
    this->drawCoordinates = std::make_shared<DrawCoordinates>();
    this->renderElements = renderElements;
}

bool HTMLView::isJustWhiteSpace(const std::shared_ptr<Node>& node)
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
        if (elementNode->tagName != "head" && elementNode->tagName != "title" && !elementNode->isDisplayNone())
        {
            if (!root)
            {
                root = std::make_shared<LayoutBox>(this->create_pango_layout(elementNode->getTextContent()), elementNode);

                if (root->element->tagName == "html")
                {
                    root->marginRect.width = this->get_width();
                    root->borderRect.width = this->get_width();
                }

                layoutBox = root;
            }
            else
            {
                if (elementNode->isDisplayBlock())
                {
                    std::shared_ptr<LayoutBox> blockBox = createBlockBox(elementNode, layoutBox);
                    blockBox->parent->children.push_back(blockBox);
                    blockBox->updateAllParentHeights();

                    layoutBox = blockBox;
                }
            }
        }
    }

    for (int i = 0; i < node->childNodes.size(); i++)
    {
        createBoxForElement(node->childNodes[i], layoutBox);
    }
}

std::shared_ptr<LayoutBox> HTMLView::createBlockBox(const std::shared_ptr<Element> elementNode, std::shared_ptr<LayoutBox> parentLayoutBox)
{
    std::shared_ptr<LayoutBox> layoutBlockBox;
    if (elementNode->tagName == "body")
    {
        // FIXME: This is a hack to strip all text from the body as it will contain other elements.
        layoutBlockBox = std::make_shared<LayoutBox>(this->create_pango_layout(""), elementNode);
    }
    else
    {
        layoutBlockBox = std::make_shared<LayoutBox>(this->create_pango_layout(elementNode->getTextContent()), elementNode);
    }

    layoutBlockBox->parent = parentLayoutBox;
    layoutBlockBox->compute();

    layoutBlockBox->marginRect.width = layoutBlockBox->parent->marginRect.width -
            layoutBlockBox->parent->borderLeftWidth - layoutBlockBox->parent->borderRightWidth -
            layoutBlockBox->parent->paddingLeft - layoutBlockBox->parent->paddingRight -
            layoutBlockBox->parent->marginLeft - layoutBlockBox->parent->marginRight;

    layoutBlockBox->borderRect.width = layoutBlockBox->parent->borderRect.width
            - layoutBlockBox->marginLeft - layoutBlockBox->marginRight
            - layoutBlockBox->parent->borderLeftWidth - layoutBlockBox->parent->borderRightWidth
            - layoutBlockBox->parent->paddingLeft - layoutBlockBox->parent->paddingRight;

    layoutBlockBox->borderRect.y += layoutBlockBox->marginTop;
    layoutBlockBox->borderRect.x += layoutBlockBox->marginLeft;

    // First element in the containing block
    if (layoutBlockBox->parent->children.empty())
    {
        layoutBlockBox->borderRect.y += layoutBlockBox->parent->borderRect.y;

        // Align layoutBlockBox in parent's layoutBlockBoxontent view
        layoutBlockBox->borderRect.y += layoutBlockBox->parent->paddingTop + layoutBlockBox->parent->borderTopWidth;
        layoutBlockBox->borderRect.x += layoutBlockBox->parent->borderRect.x + layoutBlockBox->parent->paddingLeft + layoutBlockBox->parent->borderLeftWidth;
    }
    else
    {
        // Margin Collapse Logic
        if (layoutBlockBox->parent->children.at(layoutBlockBox->parent->children.size() - 1)->marginBottom > layoutBlockBox->marginTop ||
                layoutBlockBox->parent->children.at(layoutBlockBox->parent->children.size() - 1)->marginBottom == layoutBlockBox->marginTop)
        {
          layoutBlockBox->borderRect.y -= layoutBlockBox->marginTop;
          layoutBlockBox->marginRect.height -= layoutBlockBox->marginTop;
        }

        // Align layoutBlockBox in parent's content view
        layoutBlockBox->borderRect.y += layoutBlockBox->parent->paddingTop + layoutBlockBox->parent->borderTopWidth;
        layoutBlockBox->borderRect.x += layoutBlockBox->parent->borderRect.x + layoutBlockBox->parent->paddingLeft + layoutBlockBox->parent->borderLeftWidth;

        // This is the Y position of the last block layoutBlockBox in the layoutBlockBox's containing layoutBlockBox.
        double lastBlockBoxInContainingBoxY = 0;
        for (int i = 0; i < layoutBlockBox->parent->children.size(); i++)
        {
            lastBlockBoxInContainingBoxY += layoutBlockBox->parent->children[i]->marginRect.height;
        }

        layoutBlockBox->borderRect.y += layoutBlockBox->parent->borderRect.y + lastBlockBoxInContainingBoxY;
    }

    return layoutBlockBox;

}


void HTMLView::paintLayoutBox(const Cairo::RefPtr<Cairo::Context>& cr, std::string indent, std::shared_ptr<LayoutBox> layoutBox)
{
    printf("%s\033[0;33m<LayoutBox> (%s)\033[0m\n", indent.c_str(), layoutBox->element->tagName.c_str());

    printf("%s    Containing Block: (%s)\n", indent.c_str(), layoutBox->parent ? layoutBox->parent->element->tagName.c_str() : "<Initial Containing Block>");
    printf("%s    Layout Box:\n", indent.c_str());
    printf("%s        Height: %lf\n", indent.c_str(), layoutBox->marginRect.height);
    printf("%s        Width: %lf\n", indent.c_str(), layoutBox->marginRect.width);
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

    this->maxHeight += root->marginRect.height;

    if (!isSizeSet)
    {
        set_size_request(maxWidth, maxHeight);
        isSizeSet = true;
    }


    printf("------------------------------Layout Tree------------------------------\n");
    paintLayoutBox(cr, "", root);


    return true;
}
