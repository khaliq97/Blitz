#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <string>
#include <vector>
#include <DOM/HTMLElement.h>
#include <DOM/Comment.h>
#include <DOM/DocumentType.h>
#include <DOM/Node.h>
#include <memory>
class HTMLElement;
class HTMLHeadElement;

class Document : public Node
{
    public:
        Document();
        Document(std::shared_ptr<Node> parentNode);
        std::string PrintDocument();

        //std::string URL;
        //std::string documentURI;
        //std::string contentType;

        //std::shared_ptr<DocumentType> docType;
        //std::shared_ptr<Element> documentElement;
        void dumpTree(std::shared_ptr<Node> node, std::string indent, bool last);
private:




};

#endif
