#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>
#include <memory>
#include <map>
class Document;
class DocumentType;
class Element;
class CharacterData;
class Comment;
class Text;
class LayoutBox;
class Node : public std::enable_shared_from_this<Node>
{
public:
    Node();
    Node(std::weak_ptr<Node> parentNode);
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

    std::weak_ptr<Document> ownerDocument;
    std::weak_ptr<Node> parentNode;

    std::vector<std::shared_ptr<Node>> childNodes;

    bool hasChildNodes() { return childNodes.size() > 0 ; }

    std::shared_ptr<Node> firstChild;
    std::unique_ptr<Node> lastChild;
    std::shared_ptr<Node> previousSibling;
    std::shared_ptr<Node> nextSibling;

    std::string nodeValue;
    std::string textContent;

    std::vector<std::weak_ptr<Node>> getAllNodes(std::vector<std::weak_ptr<Node> > returnNodes, std::weak_ptr<Node> node);
    std::string getIsConnected() const;

    virtual std::optional<std::shared_ptr<LayoutBox>> createLayoutBox() { return {}; }

    bool isComment() { return nodeType == NodeType::CommentNode; }
    bool isDocumentType() { return nodeType == NodeType::DocumentTypeNode; }
    bool isDocument() { return nodeType == NodeType::DocumentNode; }
    bool isElement() { return nodeType == NodeType::ElementNode; }
    bool isText() { return nodeType == NodeType::TextNode; }

    std::weak_ptr<Comment> asNodeTypeComment(std::weak_ptr<Node> comparisonNode);
    std::weak_ptr<DocumentType> asNodeTypeDocumentType(std::weak_ptr<Node> comparisonNode);
    std::weak_ptr<Element> asNodeTypeElement(std::weak_ptr<Node> comparisonNode);
    std::weak_ptr<Text> asNodeTypeText(std::weak_ptr<Node> comparisonNode);
private:
    bool m_isConnected;

};

#endif
