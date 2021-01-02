#ifndef BOX_H
#define BOX_H
#include <gtkmm.h>
class Box
{
public:
    Box();
    double blue;
    bool paint(const Cairo::RefPtr<Cairo::Context>& cr);
private:


    //const HTMLView& m_HTMLView;
};

#endif // BOX_H
