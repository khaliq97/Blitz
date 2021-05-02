#include <DOM/Node.h>
#include <DOM/Document.h>
#include <DOM/Text.h>
#include <iostream>
Node::Node()
{
    nodeType = Node::NodeType::ElementNode;
}

Node::Node(std::weak_ptr<Node> parentNode) : parentNode(parentNode)
{
    nodeType = Node::NodeType::ElementNode;
}

Node::~Node()
{
}

std::string Node::getTextContent()
{
    std::string returnStr = "";
    for (auto node: childNodes)
    {
        if (auto textNode = dynamic_cast<class Text*>(node.get()))
        {
            if (!textNode->data.empty())
            {
                returnStr += textNode->data;
            }
        }
    }
    return returnStr;
}


std::vector<std::weak_ptr<Node> > Node::getAllNodes(std::vector<std::weak_ptr<Node>> returnNodes, std::weak_ptr<Node> node)
{
    returnNodes.push_back(node);

    for (int i = 0; i < node.lock()->childNodes.size(); i++)
    {
        returnNodes = getAllNodes(returnNodes, node.lock()->childNodes[i]);
    }

    return returnNodes;
}

std::string Node::getIsConnected() const
{
    return childNodes.size() > 0 ? "true" : "false"; ;
}

std::weak_ptr<Comment> Node::asNodeTypeComment(std::weak_ptr<Node> comparisonNode)
{
    if (auto commentNode = std::dynamic_pointer_cast<Comment>(comparisonNode.lock()))
    {
        return commentNode;
    }

    return {};
}

std::weak_ptr<DocumentType> Node::asNodeTypeDocumentType(std::weak_ptr<Node> comparisonNode)
{
    if (auto documentTypeNode = std::dynamic_pointer_cast<DocumentType>(comparisonNode.lock()))
    {
        return documentTypeNode;
    }

    return {};
}

std::weak_ptr<Element> Node::asNodeTypeElement(std::weak_ptr<Node> comparisonNode)
{
    if (auto elementNode = std::dynamic_pointer_cast<Element>(comparisonNode.lock()))
    {
        return elementNode;
    }

    return {};
}

std::weak_ptr<Text> Node::asNodeTypeText(std::weak_ptr<Node> comparisonNode)
{
    if (auto textNode = std::dynamic_pointer_cast<Text>(comparisonNode.lock()))
    {
        return textNode;
    }

    return {};
}

