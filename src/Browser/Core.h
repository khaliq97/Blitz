#ifndef CASPIAN_H
#define CASPIAN_H
#include <gtkmm.h>
#include <DOM/Node.h>
#include <Layout/WebView.h>
class Core : public Gtk::Window
{
public:
    Core();
    Core(const std::shared_ptr<Node> documentWithStyling);

    std::shared_ptr<WebView> webView;

    std::unique_ptr<Gtk::MenuBar> coreMenuBar;
    std::unique_ptr<Gtk::MenuItem> menuItem_Developer;
    std::unique_ptr<Gtk::Menu> subMenu_Developer;
    std::unique_ptr<Gtk::MenuItem> menuItem_Inspector;

    std::unique_ptr<Gtk::VBox> vbox;

    const std::shared_ptr<Node> getDocumentWithStyling()
    {
        return m_document;
    }

    void inspectorMenuItemActivate();

private:
    std::shared_ptr<Node> m_document;

    bool onWindowKeyPress(GdkEventKey *event);
};


#endif // CASPIAN_H
