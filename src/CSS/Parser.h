#ifndef PARSER_H
#define PARSER_H
#include <CSS/CSSToken.h>
#include <CSS/Stylesheet.h>
#include <CSS/QualifiedRule.h>
#include <CSS/SimpleBlock.h>
#include <CSS/StyleRule.h>
#include <CSS/Selectors/ComplexSelector.h>
#include <memory>
#include <vector>
#include <CSS/Declaration.h>
#include <DOM/Document.h>
#include <LexerUtils.h>

namespace CSS {
class Parser
{
public:
    Parser(const std::vector<std::shared_ptr<CSSToken>>& tokens, const std::shared_ptr<Document>& document);
    void parseStylesheet();
    std::vector<std::shared_ptr<QualifiedRule>> parseListOfRules();
    std::vector<std::shared_ptr<StyleRule> > createStyleRules();
    std::shared_ptr<Document> document;
    std::shared_ptr<Stylesheet> styleSheet;
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

private:
    int currentCSSToken = 0;
    std::vector<std::shared_ptr<CSSToken>> tokens;
    const std::shared_ptr<CSSToken> &peek();
    const std::shared_ptr<CSSToken> &peekNext();
    void consume();
    std::vector<std::shared_ptr<QualifiedRule> > consumeListOfRules();
    bool isAtEnd();
    void notSupported(std::string tokenName);
    std::shared_ptr<QualifiedRule> consumeQualifiedRule();
    std::shared_ptr<SimpleBlock> consumeSimpleBlock();
    std::shared_ptr<CSSToken> consumeComponentValue();
    std::vector<std::shared_ptr<ComplexSelector> > parseQualifiedRulePrelude(const std::vector<std::shared_ptr<CSSToken> > &prelude);
    void printQualifiedRules();
    void printStyleRules();
    std::vector<std::shared_ptr<Declaration> > parseQualifiedRuleBlock(const std::shared_ptr<SimpleBlock> &simpleBlock);
    std::vector<std::shared_ptr<Declaration> > consumeListOfDeclarations(const std::shared_ptr<SimpleBlock> &simpleBlock);
    std::shared_ptr<Declaration> consumeDeclaration(const std::vector<std::shared_ptr<CSSToken> > &tempList);
    std::vector<std::shared_ptr<Declaration>> resolveShorthandDeclaration(std::shared_ptr<Declaration> declaration);
    std::vector<std::shared_ptr<Declaration> > resolvePosistionalShorthandDeclarationValues(ShortHandPropertyNameType type, const std::shared_ptr<Declaration> declaration);
    std::shared_ptr<Declaration> createPositionalDeclaration(std::string declarationName, const std::shared_ptr<CSSToken> &declarationValue);
};

}

#endif // PARSER_H
