#include <cairo/cairo.h>
#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm-3.0/gtkmm/drawingarea.h>
#include <Browser/Render.h>

class HTMLView : public Gtk::DrawingArea
{
    public:
        HTMLView(std::shared_ptr<Node> renderDocument);
        virtual ~HTMLView() override { }
        Glib::RefPtr<Pango::Layout> createTextLayout(const Cairo::RefPtr<Cairo::Context> & cr, const std::shared_ptr<Node> renderNode);
        void processRenderStyle(std::shared_ptr<Node> node);
        void processRenderTree();
        void drawRenderNode(Cairo::RefPtr<Cairo::Context> const & cr, std::shared_ptr<Node> node);
      protected:
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
        void drawPadding(Cairo::RefPtr<Cairo::Context> const & cr, std::shared_ptr<Node> renderNode);
        void drawMargin(Cairo::RefPtr<Cairo::Context> const & cr, std::shared_ptr<Node> renderNode);
        void drawBorder(Cairo::RefPtr<Cairo::Context> const & cr, std::shared_ptr<Node> renderNode);
        std::shared_ptr<Node> renderNodes;
        double parseLengthValue(std::string styleValue);
        std::string parseLengthUnit(std::string styleValue);
        std::vector<Glib::RefPtr<Pango::Layout>> layouts;
        bool isJustWhiteSpace(std::shared_ptr<Node> node);
        std::vector<std::shared_ptr<Node>> drawnRenderNodes;

};
