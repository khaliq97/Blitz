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
namespace CSS {
class Parser
{
public:
    Parser(const std::vector<std::shared_ptr<CSSToken>>& tokens);
    void parseStylesheet();
    std::vector<std::shared_ptr<QualifiedRule>> parseListOfRules();
    std::vector<std::shared_ptr<StyleRule> > createStyleRules();
private:
    std::vector<std::shared_ptr<CSSToken>> tokens;
    std::shared_ptr<Stylesheet> styleSheet;
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
};

}

#endif // PARSER_H
