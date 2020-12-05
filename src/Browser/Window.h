#ifndef WINDOW_H
#define WINDOW_Hm
#include <gtkmm-3.0/gtkmm.h>
#include <Browser/Render.h>


class Window : Gtk::Window
{
    public:
        Window();
        static std::unique_ptr<Gtk::Window> createBrowserWindow(std::string windowTitle, int width, int height);
        void addDrawingArea();
};


#endif // WINDOW_H
