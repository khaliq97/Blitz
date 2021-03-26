#ifndef STYLERESOLVER_H
#define STYLERESOLVER_H
#include <CSS/Declaration.h>
#include <CSS/StyleProperty.h>
#include <DOM/Document.h>

enum ShortHandPropertyNameType
{
    Margin,
    Padding,
    Border,
    BorderWidth,
    BorderStyle,
    BorderColor,
    None
};

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
    StyleResolver(const std::shared_ptr<Document>& document);
    std::shared_ptr<Document> documentWithCssDeclrations;
    std::shared_ptr<StyleProperty> findFirstDeclarationOccurence(std::string declarationName, const std::shared_ptr<Node> &node);
    std::vector<std::shared_ptr<StyleProperty>>  resolvePosistionalPropertyValues(ShortHandPropertyNameType type, const std::shared_ptr<Declaration> declaration);
    std::shared_ptr<StyleProperty> resolveStyleFromDeclaration(const std::shared_ptr<Declaration> declaration, const std::shared_ptr<Node> &node);
    std::shared_ptr<StyleProperty> createPositionalStyleProperty(std::string declarationName, const std::shared_ptr<CSSToken> &declarationValue);
    bool isColor(std::string value);
    std::shared_ptr<Color> convertColorToRGB(std::string color);
};

#endif // STYLERESOLVER_H
