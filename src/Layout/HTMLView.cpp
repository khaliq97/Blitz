#include <Layout/HTMLView.h>
#include <fmt/core.h>

HTMLView::HTMLView(std::shared_ptr<Node> documentWithResolvedStyles) : m_documentWithResolvedStyles(documentWithResolvedStyles) { }

void HTMLView::init(const Cairo::RefPtr<Cairo::Context>& cr, std::shared_ptr<Node> node, std::shared_ptr<LayoutBox> layoutBox)
{
    createLayoutTree(cr, node, layoutBox);
}

void HTMLView::createLayoutTree(const Cairo::RefPtr<Cairo::Context>& cr, std::shared_ptr<Node> node, std::shared_ptr<LayoutBox> layoutBox)
{
    if (!initialContainingBlock)
    {
        if (node->isElement())
        {
            auto element = node->asNodeTypeElement(node);

            if (element.lock()->createLayoutBox().has_value() && element.lock()->tagName() != "head")
            {
                auto initialContainingLayoutBox = element.lock()->createLayoutBox().value();
                initialContainingLayoutBox->createTextLayout(cr); // Soon to be obselete...

                initialContainingBlock = initialContainingLayoutBox;
                layoutBox = initialContainingBlock;
            }

        }
    }
    else
    {
        if (node->isElement())
        {
            auto element = node->asNodeTypeElement(node);

            if (element.lock()->createLayoutBox().has_value() && element.lock()->tagName() != "head")
            {
                  auto blockOrInlineLayoutBox = element.lock()->createLayoutBox().value();
                  blockOrInlineLayoutBox->createTextLayout(cr); // Soon to be obselete...

                  blockOrInlineLayoutBox->parent = layoutBox;

                  blockOrInlineLayoutBox->parent.lock()->children.push_back(blockOrInlineLayoutBox);
                  if (blockOrInlineLayoutBox->parent.lock()->children.size() > 1)
                      blockOrInlineLayoutBox->m_previous_sibling = blockOrInlineLayoutBox->parent.lock()->children[blockOrInlineLayoutBox->parent.lock()->children.size() - 2];

                  if (blockOrInlineLayoutBox->parent.lock()->children.size() > 1)
                      blockOrInlineLayoutBox->parent.lock()->children[blockOrInlineLayoutBox->parent.lock()->children.size() - 2]->m_next_sibling = blockOrInlineLayoutBox->parent.lock()->children[blockOrInlineLayoutBox->parent.lock()->children.size() - 1];


                  layoutBox = blockOrInlineLayoutBox;
            }
        }
    }



    for (int i = 0; i < node->childNodes.size(); i++)
    {
        createLayoutTree(cr, node->childNodes[i], layoutBox);
    }
}

void HTMLView::printLayoutTree(std::string indent, std::shared_ptr<LayoutBox> layoutBox)
{
    printf("%s\033[0;33m<LayoutBox> (%s)\033[0m\n", indent.c_str(), layoutBox->getElement()->tagName().c_str());
    printf("%s    Containing Block: (%s)\n", indent.c_str(), layoutBox->parent.lock() ? layoutBox->parent.lock()->getElement()->tagName().c_str() : "<Initial Containing Block>");
    printf("%s    Layout Box:\n", indent.c_str());
    printf("%s        Height: %lf\n", indent.c_str(), layoutBox->marginRect.height);
    printf("%s        Width: %lf\n", indent.c_str(), layoutBox->marginRect.width);
    printf("%s        Margin Top: %lf\n", indent.c_str(), layoutBox->marginTop);
    printf("%s    Border Rectangle:\n", indent.c_str());
    printf("%s        Height: %lf\n", indent.c_str(), layoutBox->borderRect.height);
    printf("%s        Width: %lf\n", indent.c_str(), layoutBox->borderRect.width);
    printf("%s        Coords (x,y): \n", indent.c_str());
    printf("%s            (%lf, %lf)\n", indent.c_str(), layoutBox->borderRect.x, layoutBox->borderRect.y);
    printf("%s    Siblings:\n", indent.c_str());

    if (layoutBox->previousSibling())
    {
        printf("%s        Previous: %s\n", indent.c_str(), layoutBox->previousSibling()->getElement()->tagName().c_str());
    }


    if (layoutBox->nextSibling())
    {
        printf("%s        Next: %s\n", indent.c_str(), layoutBox->nextSibling()->getElement()->tagName().c_str());
    }

    indent += "    ";
    for (int i = 0; i < layoutBox->children.size(); i++)
    {
        printLayoutTree(indent, layoutBox->children[i]);
    }
}

void HTMLView::paintLayoutBox(const Cairo::RefPtr<Cairo::Context>& cr, std::shared_ptr<LayoutBox> layoutBox)
{
   layoutBox->paint(cr);

   for (int i = 0; i < layoutBox->children.size(); i++)
   {
       paintLayoutBox(cr, layoutBox->children[i]);
   }
}

void HTMLView::runFormattingContext(std::shared_ptr<LayoutBox> initialContainingBlock, const Cairo::RefPtr<Cairo::Context>& cr)
{
    BlockFormattingContext blockFormattingContext;
    if (Tools::caseInsensitiveStringCompare(initialContainingBlock->getElement()->tagName(), "html"))
    {
        blockFormattingContext.runForInitialContainingBlock(initialContainingBlock, this->get_width());
    }
    else
    {
         blockFormattingContext.run(initialContainingBlock);
    }

    for (int i = 0; i < initialContainingBlock->children.size(); i++)
    {
        runFormattingContext(initialContainingBlock->children[i], cr);
    }
}

bool HTMLView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_source_rgb(255, 255, 255);
    cr->rectangle(0, 0, this->get_width(), this->get_height());
    cr->fill();
    cr->stroke();

    this->m_cairoContext = cr;
    this->set_hexpand(true);
    this->set_halign(Gtk::ALIGN_FILL);
    this->set_vexpand(true);
    this->set_valign(Gtk::ALIGN_FILL);

    initialContainingBlock = nullptr;
    init(cr, m_documentWithResolvedStyles, {});

    runFormattingContext(initialContainingBlock, cr);

    paintLayoutBox(cr, initialContainingBlock);
    printLayoutTree("", initialContainingBlock);

    this->set_size_request(-1, initialContainingBlock->marginRect.height);

    return true;
}
