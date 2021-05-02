#include <Browser/Core.h>

Browser::Core::Core(std::shared_ptr<Node> documentWithResolvedStyles, std::string htmlFilePath) : m_documentWithResolvedStyles(documentWithResolvedStyles), m_htmlFilePath(htmlFilePath)
{
    this->set_title("Caspian Web Browser");
    this->set_default_size(1280, 720);

    // Menu Bar
    coreMenuBar = std::make_unique<Gtk::MenuBar>();
    menuItem_Developer = std::make_unique<Gtk::MenuItem>();
    subMenu_Developer = std::make_unique<Gtk::Menu>();
    menuItem_Inspector = std::make_unique<Gtk::MenuItem>("Inspector");

    menuItem_Developer->set_label("Developer");
    coreMenuBar->append(*menuItem_Developer);

    subMenu_Developer->append(*menuItem_Inspector);
    menuItem_Developer->set_submenu(*subMenu_Developer);

    // Menu Item event handler set up
    menuItem_Inspector->signal_activate().connect(sigc::mem_fun(*this, &Core::inspectorMenuItemActivate));

    // Key event handler setup
    this->signal_key_press_event().connect(sigc::mem_fun(*this, &Core::onWindowKeyPress), false);

    vbox = std::make_unique<Gtk::VBox>();
    vbox->pack_start(*coreMenuBar, false, false);

    webView = std::make_unique<WebView>(m_documentWithResolvedStyles);

    vbox->add(*webView);
    this->add(*vbox);
    vbox->show();
    vbox->show_all_children();
}

void Browser::Core::inspectorMenuItemActivate()
{
    printf("Browser: Launching Inspector window\n");
}

bool Browser::Core::onWindowKeyPress(GdkEventKey* event)
{

    switch(event->keyval)
    {
        case GDK_KEY_r:
            if (event->state & GDK_CONTROL_MASK)
            {
                 printf("Reloading page event triggered\n");
                std::unique_ptr<Blitz> blitz = std::make_unique<Blitz>(m_htmlFilePath);
                blitz->loadHTML(Tools::getFileContent(blitz->htmlFilePath()));

                vbox->remove(*webView);
                webView = std::make_unique<WebView>(blitz->documentWithResolvedStyles());

                vbox->pack_start(*coreMenuBar, false, false);
                vbox->add(*webView);
                this->remove();
                this->add(*vbox);
                vbox->show();
                vbox->show_all_children();
               printf("Reload complete!\n");
            }
            break;
    }
    return false;
}
