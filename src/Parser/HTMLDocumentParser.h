#ifndef HTMLDOCUMENTPARSER_H
#define HTMLDOCUMENTPARSER_H
#include <vector>
#include <memory>
#include <Parser/HTMLToken.h>
#include <DOM/Document.h>
#include <LexerUtils.h>
class HTMLDocumentParser
{
    public:
        HTMLDocumentParser(std::vector<HTMLToken> tokens);
        void run();
        std::weak_ptr<HTMLElement> findApproriatePlaceForInsertingNode();

        std::shared_ptr<Document> document() { return m_document; }
        std::shared_ptr<Node> findElementByTagName(const Node& nodeToFind, std::string tagName);
    private:
        enum InsertionMode
        {
            Initial,
            BeforeHTML,
            BeforeHead,
            BeforeHeadNoScript,
            InHead,
            AfterHead,
            InBody,
            Text,
            InTable,
            InTableText,
            InCaption,
            InColumnGroup,
            InTableBody,
            InRow,
            InCell,
            InSelect,
            InSelectInTable,
            InTemplate,
            AfterBody,
            InFrameset,
            AfterFrameset,
            AfterAfterBody,
            AfterAfterFrameset
        };

        InsertionMode insertionMode;
        std::stack<std::weak_ptr<HTMLElement>> openElements;
        std::vector<HTMLToken> tokens;
        std::shared_ptr<Document> m_document;
        void insertHTMLElementNode(const HTMLToken& token, std::weak_ptr<Node> parentNode);
        void insertTextNode(const HTMLToken& token);
        void processInitial(const HTMLToken& token);
        void processBeforeHTML(const HTMLToken& token);
        void processBeforeHead(const HTMLToken& token);
        void processInHead(const HTMLToken& token);
        void processAfterHead(const HTMLToken& token);
        void processInBody(const HTMLToken& token);
        void processAfterBody(const HTMLToken& token);
        void processText(const HTMLToken& token);
        void switchToState(InsertionMode newMode);
};
#endif
