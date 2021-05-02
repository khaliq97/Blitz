#ifndef STYLERESOLVER_H
#define STYLERESOLVER_H
#include <CSS/Declaration.h>
#include <CSS/StyleProperty.h>
#include <DOM/Document.h>
#include <LexerUtils.h>

enum class CSSColor {
    black,
    silver,
    gray,
    white,
    maroon,
    red,
    purple,
    fuchsia,
    green,
    lime,
    olive,
    yellow,
    navy,
    blue,
    teal,
    aqua
};

class StyleResolver
{
public:
    StyleResolver(std::shared_ptr<Document>& documentWithStyling);

    std::shared_ptr<Document> documentWithResolvedStyles() { return m_documentWithResolvedStyles; }
private:
     std::shared_ptr<Document> m_documentWithResolvedStyles;

     std::shared_ptr<StyleProperty> findFirstDeclarationOccurence(std::string declarationName, const std::shared_ptr<Node> &node);
     std::shared_ptr<StyleProperty> resolveStyleFromDeclaration(const std::shared_ptr<Declaration>& declaration, const std::shared_ptr<Node> &node);
     std::shared_ptr<StyleProperty> createPositionalStyleProperty(std::string declarationName, const std::shared_ptr<CSSToken> &declarationValue);
     bool isColor(std::string value);
     std::shared_ptr<Color> convertColorToRGB(std::string color);
};

#endif // STYLERESOLVER_H
