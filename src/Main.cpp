#include <gtkmm-3.0/gtkmm.h>
#include <Terminal/Terminal.h>
#include <JS/JS.h>
#include <gtkmm.h>
#include <memory>
#include <string>
#include <stdio.h>
#include <thread>
#include <Blitz.h>

std::unique_ptr<Terminal> terminal;
std::unique_ptr<Blitz> blitzInstance;

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

    auto app = Gtk::Application::create(argc, argv, "blitz.web.browser", Gio::ApplicationFlags::APPLICATION_HANDLES_COMMAND_LINE);
    app->signal_command_line().connect(sigc::bind(sigc::ptr_fun(onCommandLine), app), false);

    terminal = std::make_unique<Terminal>();
    blitzInstance = std::make_unique<Blitz>();
    blitzInstance->loadHTML(terminal->lexer->getFileContent(argv[1]));

    //return 0;
    return app->run(*blitzInstance->browserCoreWindow);
}
