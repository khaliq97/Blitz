#ifndef BLITZ_H
#define BLITZ_H
#include <string>
#include <memory>
#include <Parser/HTMLDocumentParser.h>
#include <Parser/HTMLStateMachine.h>
#include <Parser/CSSParser.h>
#include <Browser/Render.h>
class Blitz {
    public:
        Blitz();
        std::string getFileContent(std::string fileName);
        void loadHTML(std::string html);
        std::shared_ptr<HTMLDocumentParser> documentParser;
        std::unique_ptr<HTMLStateMachine> stateMachine;
        std::unique_ptr<CSSParser> cssParser;
        std::unique_ptr<class Render> render;
};

#endif // BLITZ_H
