#include <Browser/Core.h>

Core::Core()
{

}

Core::Core(std::shared_ptr<Node> documentWithStyling)
{
    m_document = documentWithStyling;
    std::vector<std::shared_ptr<Node>> nodeList = documentWithStyling->getAllNodes({}, documentWithStyling);
    std::string windowName;
    for (auto node: nodeList)
    {
        if (node->nodeName == "title")
            windowName = node->getTextContent();
    }
    this->set_title(windowName);
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

    webView = std::make_unique<WebView>(this, m_document);

    vbox->add(*webView);
    this->add(*vbox);
    vbox->show();
    vbox->show_all_children();
}

void Core::inspectorMenuItemActivate()
{
    printf("Browser: Launching Inspector window\n");
}

bool Core::onWindowKeyPress(GdkEventKey* event)
{

    switch(event->keyval)
    {
        case GDK_KEY_r:
            if (event->state & GDK_CONTROL_MASK)
            {
                printf("Reloading page event triggered\n");
            }
            break;
    }
    return false;
}
