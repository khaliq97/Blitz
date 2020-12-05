#ifndef HTMLDOCUMENTPARSER_H
#define HTMLDOCUMENTPARSER_H
#include <vector>
#include <memory>
#include <Parser/HTMLToken.h>
#include <DOM/Document.h>
#include <Parser/Lexer.h>
class HTMLDocumentParser
{
    public:
        std::shared_ptr<Lexer> lexer;
        HTMLDocumentParser(std::string html, std::vector<std::shared_ptr<HTMLToken>> tokens);
        void run();
        void printDOMTree();
        std::shared_ptr<HTMLElement> findApproriatePlaceForInsertingNode();
        std::shared_ptr<Document> getDocument() { return document; }
        std::shared_ptr<Node> findElementByTagName(std::shared_ptr<Node> node, std::string tagName);
    private:
        std::vector<std::shared_ptr<HTMLToken>> tokens;
        std::shared_ptr<Document> document;
        std::shared_ptr<HTMLHeadElement> headElement;
        void insertCommentNode(std::shared_ptr<HTMLToken> token, std::shared_ptr<Node> parentNode);
        void insertDocumentTypeNode(std::shared_ptr<HTMLToken> token, std::shared_ptr<Node> parentNode);
        void insertHTMLElementNode(std::shared_ptr<HTMLToken> token, std::shared_ptr<Node> parentNode);
        void insertTextNode(std::shared_ptr<HTMLToken> token, std::shared_ptr<Node> parentNode);
        void processInitial(std::shared_ptr<HTMLToken> token);
        void processBeforeHTML(std::shared_ptr<HTMLToken> token);
        void processBeforeHead(std::shared_ptr<HTMLToken> token);
        void processInHead(std::shared_ptr<HTMLToken> token);
        void processAfterHead(std::shared_ptr<HTMLToken> token);
        void processInBody(std::shared_ptr<HTMLToken> token);
        void processAfterBody(std::shared_ptr<HTMLToken> token);
        void processText(std::shared_ptr<HTMLToken> token);
};
#endif
