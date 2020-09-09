#include <Browser/Render.h>
#include <Browser/HTMLView.h>
#include <Browser/Window.h>
#include <gtkmm-3.0/gtkmm.h>
#include <Terminal/Terminal.h>
#include <JS/JS.h>
#include <gtk/gtk.h>
#include <memory>
#include <string>
#include <stdio.h>
#include <curl/curl.h>
#include <thread>

std::unique_ptr<Terminal> terminal;
std::unique_ptr<Lumber> lumberInstance;

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int onCommandLine(const Glib::RefPtr<Gio::ApplicationCommandLine> &, Glib::RefPtr<Gtk::Application> &app)
{
    app->activate();
    return 0;
}

int main(int argc, char ** argv)
{
    std::string arg = argv[1];
    if (arg.compare("js") == 0)
    {
        if (argc == 3)
        {
             std::unique_ptr<JS> js = std::make_unique<JS>(argv[2]);
             return 0;
        }

        std::unique_ptr<JS> js = std::make_unique<JS>("");
        return 0;
    }

    auto app = Gtk::Application::create(argc, argv, "lumber.web.browser", Gio::ApplicationFlags::APPLICATION_HANDLES_COMMAND_LINE);
    app->signal_command_line().connect(sigc::bind(sigc::ptr_fun(onCommandLine), app), false);

    terminal = std::make_unique<Terminal>();
    lumberInstance = std::make_unique<Lumber>();
    lumberInstance->loadHTML(terminal->lexer->getFileContent(argv[1]));

    std::unique_ptr<Gtk::Window> window = Window::createBrowserWindow("Lumber Web", 1280, 720);

    std::unique_ptr<HTMLView> view = std::make_unique<HTMLView>(lumberInstance->documentParser->getDocument());
    view->processRenderTree();
    window->add(*view);
    view->show();
    return app->run(*window, argc, argv);
}
