#include <Layout/WebView.h>

WebView::WebView(Core* core, const std::shared_ptr<Node> renderElements) : m_core(core)
{
    printf("Init WebView\n");
    htmlView = std::make_unique<HTMLView>(core, renderElements);
    htmlView->set_size_request(htmlView->maxWidth, htmlView->maxHeight);


    this->add(*htmlView);
    this->show_all();
}
