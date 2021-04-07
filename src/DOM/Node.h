#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>
#include <memory>
#include <Browser/Layout.h>
class Document;
class Element;
class CharacterData;
class Node
{
    public:
        Node();
        Node(std::shared_ptr<Node> parentNode);
        virtual ~Node();
        std::string getTextContent();

        enum NodeType
        {
            ElementNode = 1,
            Attribute_Node = 2,
            TextNode = 3,
            CDATASectionNode = 4,
            EntityReferenceNode = 5,// Historical
            EntityNode = 6, // Historical
            ProcessingInstructionNode = 7,
            CommentNode = 8,
            DocumentNode = 9,
            DocumentTypeNode = 10,
            DoucmentFragmentNod = 11,
            NotationNode = 12, // Historical

        };

        NodeType nodeType;
        std::string nodeName;

        std::string baseURI;


        std::shared_ptr<Document> ownerDocument;
        std::shared_ptr<Node> parentNode;
        std::vector<std::shared_ptr<Node>> childNodes;

        bool hasChildNodes() { return childNodes.size() > 0 ; }

        std::shared_ptr<Node> firstChild;
        std::unique_ptr<Node> lastChild;
        std::shared_ptr<Node> previousSibling;
        std::shared_ptr<Node> nextSibling;

        std::string nodeValue;
        std::string textContent;

        std::shared_ptr<Layout> layout;
        std::vector<std::shared_ptr<Node>> getAllNodes(std::vector<std::shared_ptr<Node> > returnNodes, std::shared_ptr<Node> node);
        std::string getIsConnected() const;

    private:
        bool m_isConnected;

};

#endif
