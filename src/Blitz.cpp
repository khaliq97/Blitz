#include <Blitz.h>
#include <fstream>
#include <gtkmm.h>
#include <iostream>
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

    cssTokenizer = std::make_shared<Tokenizer>(lexer.getFileContent("../res/default.css"));
    cssParser = std::make_shared<CSS::Parser>(cssTokenizer->getTokens(), documentParser->getDocument());
    selectorEngine = std::make_shared<SelectorEngine>(cssParser->styleSheet, documentParser->getDocument());

    browserCoreWindow = std::make_unique<Core>(selectorEngine->documentWithStyling);
}
