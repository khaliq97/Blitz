#include <Layout/WebView.h>
#include <fmt/core.h>
WebView::WebView(std::shared_ptr<Node> documentWithResolvedStyles)
{
    htmlView = std::make_unique<HTMLView>(documentWithResolvedStyles);

    this->add(*htmlView);
    this->show_all();
}
