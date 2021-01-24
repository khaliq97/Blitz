#include <Browser/Core.h>
Core::Core()
{

}

Core::Core(const std::shared_ptr<Node> documentWithStyling) : m_document(documentWithStyling)
{
    this->set_title("Blitz Web Engine");
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

    vbox = std::make_unique<Gtk::VBox>();
    vbox->pack_start(*coreMenuBar, false, false);


    webView = std::make_shared<WebView>(this, m_document);

    vbox->add(*webView);
    this->add(*vbox);
    vbox->show();
    vbox->show_all_children();
}

void Core::inspectorMenuItemActivate()
{
    printf("Browser: Launching Inspector window\n");
}
