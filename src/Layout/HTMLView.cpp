#include <Layout/HTMLView.h>
HTMLView::HTMLView()
{
    int y = 0;
    for (int i = 0; i < 40; i++)
    {
        std::shared_ptr<Box> box = std::make_shared<Box>();
        box = std::make_shared<Box>();
        box->blue = y;
        boxes.push_back(std::move(box));
        y += 100;
    }



}

bool HTMLView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    for (auto b: boxes)
    {
        b->paint(cr);
    }
}
