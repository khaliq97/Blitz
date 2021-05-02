#include <DOM/DocumentType.h>
DocumentType::DocumentType(std::weak_ptr<Node> parentNode) :  Node(parentNode)
{
    nodeName = "Document Type";
    nodeType = Node::NodeType::DocumentTypeNode;
}

std::string DocumentType::name() const
{
    return m_name;
}

std::string DocumentType::publicId() const
{
    return m_publicId;
}

std::string DocumentType::systemId() const
{
    return m_systemId;
}
void DocumentType::setName(std::string documentTypeName)
{
    m_name = documentTypeName;
}
