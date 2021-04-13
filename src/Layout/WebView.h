#ifndef WEBVIEW_H
#define WEBVIEW_H
#include <gtkmm.h>
#include <Layout/HTMLView.h>
class Core;
class WebView : public Gtk::ScrolledWindow
{
public:
    WebView(Core* core, std::shared_ptr<Node> renderElements);
    std::shared_ptr<HTMLView> htmlView;

private:
    Core* m_core;
    void test();

};

#endif // WEBVIEW_H
