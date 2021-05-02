#ifndef CORE_H
#define CORE_H
#include <gtkmm.h>
#include <DOM/Node.h>
#include <Layout/WebView.h>
#include <Blitz.h>

namespace Browser
{
class Core : public Gtk::Window
{
public:
    Core(std::shared_ptr<Node> documentWithResolvedStyles, std::string htmlFilePath);

    std::unique_ptr<WebView> webView;

    std::unique_ptr<Gtk::MenuBar> coreMenuBar;
    std::unique_ptr<Gtk::MenuItem> menuItem_Developer;
    std::unique_ptr<Gtk::Menu> subMenu_Developer;
    std::unique_ptr<Gtk::MenuItem> menuItem_Inspector;

    std::unique_ptr<Gtk::VBox> vbox;

    std::shared_ptr<Node> getDocumentWithStyling() { return m_documentWithResolvedStyles; }

    void inspectorMenuItemActivate();

private:
    std::shared_ptr<Node> m_documentWithResolvedStyles;

    bool onWindowKeyPress(GdkEventKey *event);
    std::string m_htmlFilePath;
};
}

#endif // CORE_H
