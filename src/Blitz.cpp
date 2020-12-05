#include <Blitz.h>
#include <fstream>
#include <Browser/Render.h>
#include <gtkmm.h>
#include <Browser/Window.h>
#include <Browser/HTMLView.h>

static Lexer lexer;

Blitz::Blitz()
{

}

void Blitz::loadHTML(std::string html)
{
    stateMachine = std::make_unique<HTMLStateMachine>(html);
    stateMachine->run();

    documentParser = std::make_shared<HTMLDocumentParser>(html, stateMachine->getParsedTokens());
    documentParser->run();

    render = std::make_unique<class Render>(documentParser->getDocument(), documentParser->lexer);
    //render->render();

    std::shared_ptr<Node> styleTag = documentParser->findElementByTagName(documentParser->getDocument(), "style");

    cssParser = std::make_unique<CSSParser>(documentParser->getDocument());

    cssParser->runCSSContent(lexer.getFileContent("../res/default.css"));
}
