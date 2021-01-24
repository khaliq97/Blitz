#ifndef BLITZ_H
#define BLITZ_H
#include <string>
#include <memory>
#include <Parser/HTMLDocumentParser.h>
#include <Parser/HTMLStateMachine.h>
#include <CSS/Tokenizer.h>
#include <CSS/Parser.h>
#include <CSS/SelectorEngine.h>
#include <Layout/WebView.h>
#include <Browser/Window.h>
#include <Browser/Core.h>
class Blitz {
    public:
        Blitz();
        std::string getFileContent(std::string fileName);
        void loadHTML(std::string html);
        std::unique_ptr<HTMLStateMachine> stateMachine;
        std::shared_ptr<HTMLDocumentParser> documentParser;
        std::shared_ptr<Tokenizer> cssTokenizer;
        std::shared_ptr<CSS::Parser> cssParser;
        std::shared_ptr<SelectorEngine> selectorEngine;

        std::unique_ptr<Core> browserCoreWindow;

};

#endif // BLITZ_H
