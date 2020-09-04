#include <DOM/Node.h>
#include <DOM/Document.h>
#include <DOM/Text.h>

Node::Node()
{
    nodeType = Node::NodeType::ElementNode;
    nodeName = "";
    baseURI = "";
    isConnected = false;
    nodeValue = "";
    textContent = "";
}

Node::Node(std::shared_ptr<Node> parentNode) : parentNode(parentNode)
{
    nodeType = Node::NodeType::ElementNode;
    nodeName = "";
    baseURI = "";
    isConnected = false;
    nodeValue = "";
    textContent = "";
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
