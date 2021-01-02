#ifndef WEBVIEW_H
#define WEBVIEW_H
#include <gtkmm.h>
#include <Layout/HTMLView.h>
class WebView : public Gtk::ScrolledWindow
{
public:
    WebView();

private:

    std::shared_ptr<HTMLView> htmlView;
};

#endif // WEBVIEW_H
