#ifndef DOCUMENTTYPE_H
#define DOCUMENTTYPE_H
#include <DOM/Node.h>
#include <string>

class DocumentType : public Node
{
    public:
        DocumentType(std::shared_ptr<Node> parentNode);
        std::string name;
        std::string publicId;
        std::string systemId;
};


#endif // DOCUMENTTYPE_H
