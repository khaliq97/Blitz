#include <Layout/WebView.h>
WebView::WebView()
{
    printf("Init WebView\n");
    htmlView = std::make_unique<HTMLView>();
    htmlView->set_size_request(1920, 5000);

    this->add(*htmlView);
    this->show_all();
}
