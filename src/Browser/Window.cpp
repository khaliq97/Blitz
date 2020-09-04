#include <Browser/Window.h>
#include <Browser/HTMLView.h>
#include <string>
#include <memory>

Window::Window() {}

std::unique_ptr<Gtk::Window> Window::createBrowserWindow(std::string windowTitle, int width, int height)
{
    std::unique_ptr<Gtk::Window> window = std::make_unique<Gtk::Window>();
    window->resize(width, height);
    window->set_title(windowTitle);

    return window;
}


