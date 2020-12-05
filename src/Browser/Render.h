#ifndef RENDER_H
#define RENDER_H
#include <DOM/Document.h>
#include <Parser/Lexer.h>
#include <vector>
#include <memory>
class Render
{
    public:
        std::shared_ptr<Lexer> lexer;
        Render(std::shared_ptr<Document> document, std::shared_ptr<Lexer> lexer);
        std::vector<std::shared_ptr<Node>> renderNodes;
        //void render();
        void processDOMNode(std::shared_ptr<Node> node, bool last);
        std::shared_ptr<Document> document;
        void appendRenderNode(std::shared_ptr<Node> renderNode);
};

#endif // RENDER_H
