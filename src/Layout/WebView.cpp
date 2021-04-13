#include <Layout/WebView.h>

WebView::WebView(Core* core, std::shared_ptr<Node> renderElements) : m_core(core)
{
    printf("Init WebView\n");

    htmlView = std::make_unique<HTMLView>(core, renderElements);

    this->add(*htmlView);
    this->show_all();
}
