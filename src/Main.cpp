#include <gtkmm-3.0/gtkmm.h>
#include <Terminal/Terminal.h>
#include <JS/JS.h>
#include <gtkmm.h>
#include <memory>
#include <string>
#include <stdio.h>
#include <thread>
#include <Blitz.h>
#include <Browser/Core.h>
#include <fmt/core.h>
#include <fmt/color.h>

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

    std::unique_ptr<Blitz> blitzWebEngineInstance = std::make_unique<Blitz>(argv[1]);
    blitzWebEngineInstance->loadHTML(Tools::getFileContent(argv[1]));

    std::unique_ptr<Browser::Core> browserCoreWindow = std::make_unique<Browser::Core>(blitzWebEngineInstance->documentWithResolvedStyles(), blitzWebEngineInstance->htmlFilePath());
    browserCoreWindow->set_icon_from_file("../res/Blitz Logo V1.png");


    return app->run(*browserCoreWindow);
}
