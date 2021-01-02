#ifndef STYLERESOLVER_H
#define STYLERESOLVER_H
#include <CSS/Declaration.h>
#include <CSS/StyleProperty.h>
#include <DOM/Document.h>
class StyleResolver
{
public:
    StyleResolver(const std::shared_ptr<Document>& document);
    std::shared_ptr<Document> documentWithCssDeclrations;
    std::shared_ptr<StyleProperty> resolveStyleFromDeclaration(const std::shared_ptr<Declaration> declaration, const std::shared_ptr<Node> &node);
    std::shared_ptr<StyleProperty> findFirstDeclarationOccurence(std::string declarationName, const std::shared_ptr<Node> &node);
};

#endif // STYLERESOLVER_H
