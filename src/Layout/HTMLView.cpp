#include <Layout/HTMLView.h>
HTMLView::HTMLView(Core* core, const std::shared_ptr<Node> renderElements) : m_core(core)
{
    this->drawCoordinates = std::make_shared<DrawCoordinates>();
    this->renderElements = renderElements;

    createBoxForElement(renderElements);

    for (auto b: boxes)
    {
        b->compute();
        printf("Box Height: %lf\n", b->height);
        printf("Box Width: %lf\n", b->width);
        if (b->height > 10000)
            printf("Name that is overflowing: %s\n", b->element->getTextContent().c_str());
        this->maxHeight += b->height;
        if (b->width > this->maxWidth)
            this->maxWidth = b->width;
    }



    printf("Viewport Height: %lf\n", maxHeight);
    printf("Viewport Width: %lf\n", maxWidth);

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

void HTMLView::createBoxForElement(std::shared_ptr<Node> node)
{
    if (auto elementNode = std::dynamic_pointer_cast<Element>(node))
    {
        if (!isJustWhiteSpace(node) && elementNode->tagName != "title")
        {
            std::shared_ptr<Box> box = std::make_shared<Box>(this->create_pango_layout(elementNode->getTextContent()), drawCoordinates, elementNode);
            this->boxes.push_back(std::move(box));
        }

    }

    for (int i = 0; i < node->childNodes.size(); i++)
    {
        createBoxForElement(node->childNodes[i]);
    }
}

bool HTMLView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    // Make the background of the view white

    this->set_hexpand(true);
    this->set_halign(Gtk::ALIGN_FILL);
    this->set_vexpand(true);
    this->set_valign(Gtk::ALIGN_FILL);

    printf("HTMLView Widget width: %d\n", this->get_width());
    printf("HTMLView Widget height: %d\n", this->get_height());

    cr->set_source_rgb(255, 255, 255);
    cr->rectangle(0, 0, this->get_width(), this->get_height());
    cr->fill();
    cr->stroke();

    drawCoordinates->x = 0;
    drawCoordinates->y = 0;

    int i = 0;
    for (auto b: boxes)
    {
        b->localDrawX = drawCoordinates->x;
        b->localDrawY = drawCoordinates->y;

        if (b->element->getStylePropertyByDeclarationName("display")->m_declaration->value[0]->value() == "block")
        {
            drawCoordinates->y += b->height;
        }
        else {
            drawCoordinates->x += b->width;
        }
        b->paint(cr);
        i++;

    }

    return true;
}
