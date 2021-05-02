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
    Parser(std::vector<CSSToken> tokens);
    std::unique_ptr<Stylesheet> styleSheet() { return std::move(m_styleSheet); }
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
    std::unique_ptr<Stylesheet> m_styleSheet;
    std::vector<CSSToken> tokens;
    const CSSToken& peek();
    const CSSToken& peekNext();
    void consume();
    std::vector<std::shared_ptr<QualifiedRule> > consumeListOfRules();
    bool isAtEnd();
    void notSupported(std::string tokenName);
    std::shared_ptr<QualifiedRule> consumeQualifiedRule();
    std::shared_ptr<SimpleBlock> consumeSimpleBlock();
    CSSToken consumeComponentValue();
    std::vector<std::shared_ptr<ComplexSelector> > parseQualifiedRulePrelude(const std::vector<CSSToken>& prelude);
    void printQualifiedRules();
    void printStyleRules();
    std::vector<std::shared_ptr<Declaration>> parseQualifiedRuleBlock(const std::shared_ptr<SimpleBlock> &simpleBlock);
    std::vector<std::shared_ptr<Declaration>> consumeListOfDeclarations(const std::shared_ptr<SimpleBlock> &simpleBlock);
    std::vector<std::shared_ptr<Declaration>> resolveShorthandDeclaration(std::shared_ptr<Declaration> declaration);
    std::vector<std::shared_ptr<Declaration>> resolvePosistionalShorthandDeclarationValues(ShortHandPropertyNameType type, const std::shared_ptr<Declaration> declaration);
    std::shared_ptr<Declaration> createPositionalDeclaration(std::string declarationName, const CSSToken declarationValue);
    std::shared_ptr<Declaration> consumeDeclaration(const std::vector<CSSToken>& tempList);
    void parseStylesheet();
    std::vector<std::shared_ptr<QualifiedRule>> parseListOfRules();
    std::vector<std::shared_ptr<StyleRule>> createStyleRules();
};

}

#endif // PARSER_H
