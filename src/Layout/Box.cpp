#include <Layout/Box.h>

Box::Box()
{
}

bool Box::paint(const Cairo::RefPtr<Cairo::Context> &cr)
{
    printf("DRAWING!!\n");
    cr->set_source_rgb(0.0, 0.0, 255);
    cr->rectangle(0, blue, 100, 100);
    cr->fill();
}
