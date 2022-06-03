#pragma once
#include <DOM/Element.h>
#include <gtkmm.h>
#include <Layout/LayoutBox.h>
#include <DOM/Text.h>
#include <Layout/BlockFormattingContext.h>
#include <LexerUtils.h>
class Core;
class HTMLView : public Gtk::DrawingArea
{
public:
    HTMLView(std::shared_ptr<Node> documentWithResolvedStyles);
    void init(const Cairo::RefPtr<Cairo::Context>& cairoContext, std::shared_ptr<Node> node, std::shared_ptr<LayoutBox> layoutBox);

    double totalPageHeight = 0;
    double maxWidth = 0;
private:

    Cairo::RefPtr<Cairo::Context> m_cairoContext;
    Glib::RefPtr<Pango::Layout> createTextLayout(const std::shared_ptr<Node> renderNode);
    bool isJustWhiteSpace(const std::shared_ptr<Node>& node);
    std::shared_ptr<Node> m_documentWithResolvedStyles;
    std::vector<std::shared_ptr<LayoutBox>> boxes;
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    std::shared_ptr<LayoutBox> initialContainingBlock;
    void createBoxForElement(const Cairo::RefPtr<Cairo::Context>& cr, std::shared_ptr<Node> node, std::shared_ptr<LayoutBox> layoutBox);
    std::shared_ptr<LayoutBox> createBlockBox(const Cairo::RefPtr<Cairo::Context>& cr, std::shared_ptr<Element> elementNode, std::shared_ptr<LayoutBox> parentLayoutBox);
    void createLayoutTree(const Cairo::RefPtr<Cairo::Context>& cr, std::shared_ptr<Node> node, std::shared_ptr<LayoutBox> layoutBox);
    void runFormattingContext(std::shared_ptr<LayoutBox> initialContainingBlock, const Cairo::RefPtr<Cairo::Context>& cr);
    void printLayoutTree(std::string indent, std::shared_ptr<LayoutBox> layoutBox);
    void paintLayoutBox(const Cairo::RefPtr<Cairo::Context> &cr, std::shared_ptr<LayoutBox> layoutBox);
};
