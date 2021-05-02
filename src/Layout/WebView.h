#ifndef WEBVIEW_H
#define WEBVIEW_H
#include <gtkmm.h>
#include <Layout/HTMLView.h>
class Core;
class WebView : public Gtk::ScrolledWindow
{
public:
    WebView(std::shared_ptr<Node> documentWithResolvedStyles);
    std::shared_ptr<HTMLView> htmlView;

private:
    void test();

};

#endif // WEBVIEW_H
