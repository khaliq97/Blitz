#include <Blitz.h>
#include <fstream>
#include <gtkmm.h>
#include <iostream>

Blitz::Blitz()
{

}

void Blitz::loadHTML(std::string html)
{
    stateMachine = std::make_unique<HTMLStateMachine>(html);
    stateMachine->run();


    documentParser = std::make_unique<HTMLDocumentParser>(stateMachine->getParsedTokens());
    documentParser->run();

    cssTokenizer = std::make_unique<Tokenizer>(Tools::getFileContent("../res/default.css"));
    cssParser = std::make_unique<CSS::Parser>(cssTokenizer->getTokens(), documentParser->getDocument());
    selectorEngine = std::make_unique<SelectorEngine>(cssParser->styleSheet, documentParser->getDocument());


    browserCoreWindow = std::make_unique<Core>(selectorEngine->documentWithStyling);
}
