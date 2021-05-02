#ifndef DOCUMENTTYPE_H
#define DOCUMENTTYPE_H
#include <DOM/Node.h>
#include <string>

class DocumentType : public Node
{
public:
    DocumentType(std::weak_ptr<Node> parentNode);
    std::string name() const;
    std::string publicId() const;
    std::string systemId() const;
    void setName(std::string documentTypeName);
private:
    std::string m_name = "";
    std::string m_publicId = "";
    std::string m_systemId = "";
};


#endif // DOCUMENTTYPE_H
