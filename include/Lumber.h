#ifndef LUMBER_H
#define LUMBER_H
#include <string>
#include <memory>
#include <Parser/HTMLDocumentParser.h>
#include <Parser/HTMLStateMachine.h>
#include <Parser/CSSParser.h>
#include <Browser/Render.h>
class Lumber {
    public:
        Lumber();
        std::string getFileContent(std::string fileName);
        void loadHTML(std::string html);
        std::shared_ptr<HTMLDocumentParser> documentParser;
        std::unique_ptr<HTMLStateMachine> stateMachine;
        std::unique_ptr<CSSParser> cssParser;
        std::unique_ptr<class Render> render;
};

#endif // LUMBER_H
