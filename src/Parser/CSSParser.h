#ifndef CSSPARSER_H
#define CSSPARSER_H
#include <string>
#include <Parser/StyleRule.h>
#include <Parser/Stylesheet.h>
#include <Browser/RenderNode.h>
#include <DOM/Document.h>
class CSSParser
{
    public:
        CSSParser(std::shared_ptr<Document> document);
        char peek();
        char consume();
        void consumeWhitespace();
        void runCSSContent(std::string cssContent);
        char consumeChar();
        std::string consumeUntilChar(char tokenData);
        void runSelectorEngine();
        bool caseInsensitiveStringCompare(std::string val1, std::string val2);

        std::shared_ptr<Stylesheet> styleSheet;
        std::shared_ptr<Document> document;
        std::vector<std::shared_ptr<RenderNode>> renderTree;
        void applyAllRule(std::shared_ptr<Node> node, std::string type, std::shared_ptr<StyleRule> styleRules);
        void applyTypeRule(std::shared_ptr<Node> node, std::string type, std::shared_ptr<StyleRule> styleRules);
        void applyClassRule(std::shared_ptr<Node> node, std::string type, std::shared_ptr<StyleRule> styleRules);
        void printRenderTree();
        void printNode(std::shared_ptr<Node> node, std::string indent, bool last);
        bool isJustWhiteSpace(std::shared_ptr<Node> node);

    private:
        std::string css;
};

#endif // CSSPARSER_H
