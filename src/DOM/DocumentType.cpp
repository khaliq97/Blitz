#include <DOM/DocumentType.h>
DocumentType::DocumentType(std::shared_ptr<Node> parentNode) :  Node(parentNode)
{
    this->name = "";
    this->publicId = "";
    this->systemId = "";
}
