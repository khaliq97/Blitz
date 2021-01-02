#ifndef WINDOW_H
#define WINDOW_Hm
#include <gtkmm-3.0/gtkmm.h>
#include <Layout/WebView.h>

class Window : public Gtk::Window
{
    public:
        Window();
        static std::unique_ptr<Gtk::Window> createBrowserWindow(std::string windowTitle, int width, int height);

    private:
        std::shared_ptr<WebView> m_webView;
};


#endif // WINDOW_H
