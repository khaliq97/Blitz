#include <Browser/Window.h>
#include <string>
#include <memory>

Window::Window()
{
    this->set_title("Blitz Web Engine");
    this->set_default_size(1280, 720);

    m_webView = std::make_shared<WebView>();
    this->add(*m_webView);
    m_webView->show();
}

std::unique_ptr<Gtk::Window> Window::createBrowserWindow(std::string windowTitle, int width, int height)
{
    std::unique_ptr<Gtk::Window> window = std::make_unique<Gtk::Window>();
    window->resize(width, height);
    window->set_title(windowTitle);

    return window;
}


