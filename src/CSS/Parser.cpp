#include <CSS/Parser.h>
int currentCSSToken = 0;
CSS::Parser::Parser(const std::vector<std::shared_ptr<CSSToken>> &tokens, const std::shared_ptr<Document>& document)
{
    this->tokens = tokens;
    this->document = document;
    styleSheet = std::make_shared<Stylesheet>();
    styleSheet->rules = parseListOfRules();
    styleSheet->styleRules = createStyleRules();
    //printQualifiedRules();
    printStyleRules();
}

void CSS::Parser::parseStylesheet()
{
    styleSheet->rules = parseListOfRules();
    styleSheet->styleRules = createStyleRules();
}

void CSS::Parser::printQualifiedRules()
{
    for (auto rules: styleSheet->rules)
    {
        printf("Qualified Rule\n");
        for (auto prelude: rules->prelude)
        {
                printf("    Prelude: %s\n", prelude->value().c_str());
                printf("    Type: %s\n", CSSTokenTypes[(int) prelude->type].c_str());
        }

        printf("    Simple Block:\n");

        for (auto simpleBlockValue: rules->simpleBlock->values)
        {
            printf("        Type: %s\n", CSSTokenTypes[(int) simpleBlockValue->type].c_str());
            printf("        Value: %s\n", simpleBlockValue->value().c_str());
            if (simpleBlockValue->type == CSSTokenType::Dimension)
            {
                printf("            Unit: %s\n", simpleBlockValue->unit.c_str());
            }
        }
    }
}

std::vector<std::shared_ptr<QualifiedRule>> CSS::Parser::parseListOfRules()
{
    return consumeListOfRules();
}

std::vector<std::shared_ptr<QualifiedRule>> CSS::Parser::consumeListOfRules()
{
    std::vector<std::shared_ptr<QualifiedRule>> rules;
    while (!isAtEnd()) {
        switch (tokens[currentCSSToken]->type)
        {

            case CSSTokenType::Whitespace:
                consume();
                break;
            case CSSTokenType::EndOfFile:
                return rules;
            //case CDOToken
            case CSSTokenType::CDC:
                notSupported("CDCToken");
                break;
            //case at-keyword-token
            default:
                std::shared_ptr<QualifiedRule> rule = consumeQualifiedRule();
                rules.push_back(rule);
                break;
        }
    }

    return rules;
}

std::shared_ptr<QualifiedRule> CSS::Parser::consumeQualifiedRule()
{
    std::shared_ptr<QualifiedRule> qualifiedRule = std::make_shared<QualifiedRule>();
    while (!isAtEnd()) {
        switch (tokens[currentCSSToken]->type)
        {
            case CSSTokenType::EndOfFile:
                printf("Parser: Parse error!\n");
                exit(0);
            case CSSTokenType::LeftCurlyBracket:
                qualifiedRule->simpleBlock = consumeSimpleBlock();
                return qualifiedRule;
            // simple block with an associated token of <{-token>??
            default:
                qualifiedRule->prelude.push_back(consumeComponentValue());
                consume();
                break;

        }
    }

    return qualifiedRule;
}

std::shared_ptr<SimpleBlock> CSS::Parser::consumeSimpleBlock()
{
    std::shared_ptr<SimpleBlock> simpleBlock = std::make_shared<SimpleBlock>();

    while (!isAtEnd()) {
        switch (tokens[currentCSSToken]->type)
        {
            case CSSTokenType::RightCurlyBracket:
                consume();
                return simpleBlock;
            case CSSTokenType::EndOfFile:
                printf("Parser: Parse Error!");
                exit(0);
            default:
                simpleBlock->values.push_back(consumeComponentValue());
                consume();
                break;

        }
    }

    return simpleBlock;
}

std::shared_ptr<CSSToken> CSS::Parser::consumeComponentValue()
{
    /*
     * If the current input token is a <{-token>, <[-token>, or <(-token>, consume a simple block and return it.
     *Otherwise, if the current input token is a <function-token>, consume a function and return it.
     */

    return tokens[currentCSSToken];

}

void CSS::Parser::printStyleRules()
{
    printf("\n------------------CSS style rules: ------------------\n");
    for (auto styleRule: this->styleSheet->styleRules)
    {
         printf("-------Style Rule-------\n");
        for (auto complexSelector: styleRule->complexSelectorList)
        {
            printf("Complex Selector:\n");
            if (complexSelector->compoundSelector)
            {
                printf("    Compound Selector\n");
                if (complexSelector->compoundSelector->typeSelector)
                {
                    printf("        Type Selector\n");
                    printf("        Ident: %s\n", complexSelector->compoundSelector->typeSelector->identToken->value().c_str());
                }
            }

            if (complexSelector->compoundSelector->subClassSelectors.size() > 0)
            {
                printf("        Sub class selectors\n");
                for (auto subClassSelector: complexSelector->compoundSelector->subClassSelectors)
                {
                    if (auto idSelector = dynamic_cast<IdSelector*>(subClassSelector.get()))
                    {
                        printf("            ID Selector\n");
                        printf("            Hash: %s\n", idSelector->hashToken->value().c_str());
                    }

                    if (auto classSelector = dynamic_cast<ClassSelector*>(subClassSelector.get()))
                    {
                        printf("            Class Selector\n");
                        printf("            Ident: %s\n", classSelector->identToken->value().c_str());
                    }
                }
            }

        }

        printf("    Declarations:\n");
        for (auto declaration: styleRule->declaration)
        {
            printf("        Declaration\n");
            printf("            Name: %s\n", declaration->name.c_str());
            for (auto declarationValue: declaration->value)
            {
                printf("            Value: %s\n", declarationValue->value().c_str());
                if (declarationValue->type == CSSTokenType::Dimension)
                    printf("                Unit: %s\n", declarationValue->unit.c_str());
            }

        }

    }
}

std::vector<std::shared_ptr<CSS::StyleRule>> CSS::Parser::createStyleRules()
{
    std::vector<std::shared_ptr<StyleRule>> styleRules;

    for(auto qualifiedRule: styleSheet->rules)
    {
        std::shared_ptr<StyleRule> styleRule = std::make_shared<StyleRule>();
        styleRule->complexSelectorList = parseQualifiedRulePrelude(qualifiedRule->prelude);
        styleRule->declaration = parseQualifiedRuleBlock(qualifiedRule->simpleBlock);

        styleRules.push_back(std::move(styleRule));
    }

    return styleRules;
}

std::vector<std::shared_ptr<ComplexSelector>> CSS::Parser::parseQualifiedRulePrelude(const std::vector<std::shared_ptr<CSSToken>>& preludes)
{
    std::vector<std::shared_ptr<ComplexSelector>> complexSelectorList;
    std::shared_ptr<ComplexSelector> currentComplexSelector = std::make_shared<ComplexSelector>();
    std::shared_ptr<CompoundSelector> currentCompoundSelector = std::make_shared<CompoundSelector>();
    bool typeSelectorAdded = false;
    bool isNextSelectorOfTypeClass = false;

    for (auto prelude: preludes)
    {
        if (prelude->type == CSSTokenType::Comma)
        {
            //Reset current selectors
            currentComplexSelector->compoundSelector = std::move(currentCompoundSelector);
            complexSelectorList.push_back(std::move(currentComplexSelector));
            currentComplexSelector = std::make_shared<ComplexSelector>();
            currentCompoundSelector = std::make_shared<CompoundSelector>();
            typeSelectorAdded = false;
            isNextSelectorOfTypeClass = false;
            continue;
        }

        // <type-selector> = <wq-name> | <ns-prefix>? '*'
        // <wq-name> = <ns-prefix>? <ident-token>
        // We are only checking for the ident token here. Ns-prefix support needs to be added
        if (prelude->type == CSSTokenType::IdentLike && !typeSelectorAdded && !isNextSelectorOfTypeClass)
        {
            std::shared_ptr<TypeSelector> typeSelector = std::make_shared<TypeSelector>(prelude);
            typeSelectorAdded = true;
            currentCompoundSelector->typeSelector = std::move(typeSelector);
            continue;
        }
        else if (prelude->type == CSSTokenType::IdentLike && typeSelectorAdded && !isNextSelectorOfTypeClass)
        {
            printf("Parsing selector returned failure\n!");
            exit(0);
        }

        //<subclass-selector> = <id-selector> | <class-selector> |
        //                      <attribute-selector> | <pseudo-class-selector>
        // <id-selector> = <hash-token>
        if (prelude->type == CSSTokenType::Hash)
        {
            std::shared_ptr<IdSelector> idSelector = std::make_shared<IdSelector>(prelude);
            currentCompoundSelector->subClassSelectors.push_back(std::move(idSelector));
            continue;
        }

        // <class-selector> = '.' <ident-token>
        if (prelude->type == CSSTokenType::Delim && prelude->value() == ".")
        {
            isNextSelectorOfTypeClass = true;
            continue;
        }

        if (isNextSelectorOfTypeClass && prelude->type == CSSTokenType::IdentLike)
        {
            std::shared_ptr<ClassSelector> classSelector = std::make_shared<ClassSelector>(prelude);
            currentCompoundSelector->subClassSelectors.push_back(std::move(classSelector));
            isNextSelectorOfTypeClass = false;
            continue;
        }
    }

    currentComplexSelector->compoundSelector = std::move(currentCompoundSelector);
    complexSelectorList.push_back(std::move(currentComplexSelector));
    return complexSelectorList;
}

std::vector<std::shared_ptr<Declaration>> CSS::Parser::parseQualifiedRuleBlock(const std::shared_ptr<SimpleBlock> &simpleBlock)
{
    return consumeListOfDeclarations(simpleBlock);
}

std::vector<std::shared_ptr<Declaration>> CSS::Parser::consumeListOfDeclarations(const std::shared_ptr<SimpleBlock> &simpleBlock)
{
    std::vector<std::shared_ptr<Declaration>> declarations;
    int simpleBlockTokenIndex = 0;
    for(auto simpleBlockToken: simpleBlock->values)
    {
        switch (simpleBlockToken->type)
        {
            case CSSTokenType::LeftCurlyBracket:
            case CSSTokenType::Whitespace:
            case CSSTokenType::Semicolon:
                 simpleBlockTokenIndex++;
                 continue;
            //<at-keyword-token>
            case CSSTokenType::IdentLike:
                std::vector<std::shared_ptr<CSSToken>> tempList;

                if (simpleBlockTokenIndex < simpleBlock->values.size() &&
                        simpleBlock->values[simpleBlockTokenIndex]->type != CSSTokenType::Semicolon &&
                        simpleBlock->values[simpleBlockTokenIndex]->type != CSSTokenType::EndOfFile)
                {
                    tempList.push_back(simpleBlock->values[simpleBlockTokenIndex]);
                }

                while(simpleBlockTokenIndex + 1 < simpleBlock->values.size())
                {
                    if (simpleBlock->values[simpleBlockTokenIndex + 1]->type != CSSTokenType::Semicolon &&
                            simpleBlock->values[simpleBlockTokenIndex + 1]->type != CSSTokenType::EndOfFile)
                    {
                        tempList.push_back(simpleBlock->values[simpleBlockTokenIndex + 1]);
                        simpleBlockTokenIndex++;
                    }
                    else
                    {
                       declarations.push_back(consumeDeclaration(tempList));
                       break;
                    }
                }

                break;
        }
    }

    return declarations;
}

std::shared_ptr<Declaration> CSS::Parser::consumeDeclaration(const std::vector<std::shared_ptr<CSSToken>> &tempList)
{
    std::shared_ptr<Declaration> declaration = std::make_shared<Declaration>();
    bool isDeclarationNameSet = false;
    bool isDeclarationValueNext = false;

    for(auto simpleBlockToken: tempList)
    {
         // First token will always be an ident like
        if (simpleBlockToken->type == CSSTokenType::IdentLike && !isDeclarationNameSet)
        {
            declaration = std::make_shared<Declaration>();
            declaration->name = simpleBlockToken->value();
            isDeclarationNameSet = true;
            continue;
        }

        if (simpleBlockToken->type == CSSTokenType::Whitespace)
        {
            continue;
        }

        if (simpleBlockToken->type == CSSTokenType::Colon)
        {
            isDeclarationValueNext = true;
            continue;
        }
        else if (!isDeclarationValueNext)
        {
           printf("PARSE ERROR!\n");
           exit(0);
        }

        if (simpleBlockToken->type != CSSTokenType::EndOfFile)
        {
            declaration->value.push_back(simpleBlockToken);
            isDeclarationNameSet = false;
            isDeclarationValueNext = false;
            continue;
        }

        // Handle !important attributes here...

        if (simpleBlockToken->type == CSSTokenType::Whitespace)
            continue;

    }

    return declaration;
}

const std::shared_ptr<CSSToken>& CSS::Parser::peek()
{
    return tokens[currentCSSToken];
}

const std::shared_ptr<CSSToken>& CSS::Parser::peekNext()
{
    return tokens[currentCSSToken + 1];
}

void CSS::Parser::consume()
{
    currentCSSToken++;
}

bool CSS::Parser::isAtEnd()
{
    return currentCSSToken >= tokens.size();
}

void CSS::Parser::notSupported(std::string tokenName)
{
    printf("Parser: %s are not supported :(\n", tokenName.c_str());
    exit(0);
}
