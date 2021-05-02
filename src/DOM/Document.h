#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <string>
#include <vector>
#include <DOM/HTMLElement.h>
#include <DOM/Comment.h>
#include <DOM/DocumentType.h>
#include <DOM/Node.h>
#include <DOM/Text.h>
#include <memory>
class HTMLElement;
class HTMLHeadElement;

class Document : public Node
{
public:
    Document();
    Document(std::weak_ptr<Node> parentNode);

    //std::string URL;
    //std::string documentURI;
    //std::string contentType;

    //std::shared_ptr<DocumentType> docType;
    //std::shared_ptr<Element> documentElement;
    void printDocument(const std::shared_ptr<Node>& node, std::string indentation);
private:
};

#endif
