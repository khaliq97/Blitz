#include <CSS/Parser.h>

CSS::Parser::Parser(std::vector<CSSToken> tokens)
{
    this->tokens = tokens;
    m_styleSheet = std::make_unique<Stylesheet>();
    m_styleSheet->rules = parseListOfRules();
    m_styleSheet->styleRules = createStyleRules();
    //printStyleRules();
}

void CSS::Parser::parseStylesheet()
{
    m_styleSheet->rules = parseListOfRules();
    m_styleSheet->styleRules = createStyleRules();
}

void CSS::Parser::printQualifiedRules()
{
    for (auto rules: m_styleSheet->rules)
    {
        printf("Qualified Rule\n");
        for (auto prelude: rules->prelude)
        {
                printf("    Prelude: %s\n", prelude.value().c_str());
                printf("    Type: %s\n", CSSTokenTypes[(int) prelude.type].c_str());
        }

        printf("    Simple Block:\n");

        for (auto simpleBlockValue: rules->simpleBlock->values)
        {
            printf("        Type: %s\n", CSSTokenTypes[(int) simpleBlockValue.type].c_str());
            printf("        Value: %s\n", simpleBlockValue.value().c_str());
            if (simpleBlockValue.type == CSSTokenType::Dimension)
            {
                printf("            Unit: %s\n", simpleBlockValue.unit.c_str());
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
        switch (tokens[currentCSSToken].type)
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
        switch (tokens[currentCSSToken].type)
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
        switch (tokens[currentCSSToken].type)
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

CSSToken CSS::Parser::consumeComponentValue()
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
    for (auto styleRule: this->m_styleSheet->styleRules)
    {
         printf("-------Style Rule-------\n");
        for (auto complexSelector: styleRule->complexSelectorList)
        {
            printf("Complex Selector:\n");
            if (complexSelector->compoundSelector())
            {
                printf("    Compound Selector\n");
                if (complexSelector->compoundSelector()->m_typeSelector)
                {
                    printf("        Type Selector\n");
                    printf("        Ident: %s\n", complexSelector->compoundSelector()->m_typeSelector->value().c_str());
                }
            }

            if (complexSelector->compoundSelector()->m_subClassSelectors.size() > 0)
            {
                printf("        Sub class selectors\n");
                for (auto subClassSelector: complexSelector->compoundSelector()->m_subClassSelectors)
                {
                    if (subClassSelector->type() == SelectorType::Id)
                    {
                        printf("            ID Selector\n");
                        printf("            Hash: %s\n", subClassSelector->value().c_str());
                    }

                    if (subClassSelector->type() == SelectorType::Class)
                    {
                        printf("            Class Selector\n");
                        printf("            Ident: %s\n", subClassSelector->value().c_str());
                    }
                }
            }
            printf("    Selector Specifity Data: \n");

            printf("        A: %d\n", complexSelector->a);
            printf("        B: %d\n", complexSelector->b);

            printf("        C: %d\n", complexSelector->c);


        }

        printf("    Declarations:\n");
        for (auto declaration: styleRule->declaration)
        {
            printf("        Declaration\n");
            printf("            Name: %s\n", declaration->name.c_str());
            for (auto declarationValue: declaration->value)
            {
                printf("            Value: %s\n", declarationValue.value().c_str());
                if (declarationValue.type == CSSTokenType::Dimension)
                    printf("                Unit: %s\n", declarationValue.unit.c_str());
            }

        }

    }
}

std::vector<std::shared_ptr<CSS::StyleRule>> CSS::Parser::createStyleRules()
{
    std::vector<std::shared_ptr<StyleRule>> styleRules;

    for(auto qualifiedRule: m_styleSheet->rules)
    {
        std::shared_ptr<StyleRule> styleRule = std::make_shared<StyleRule>();
        styleRule->complexSelectorList = parseQualifiedRulePrelude(qualifiedRule->prelude);
        styleRule->declaration = parseQualifiedRuleBlock(qualifiedRule->simpleBlock);

        styleRules.push_back(std::move(styleRule));
    }

    return styleRules;
}

std::vector<std::shared_ptr<ComplexSelector>> CSS::Parser::parseQualifiedRulePrelude(const std::vector<CSSToken>& preludes)
{
    std::vector<std::shared_ptr<ComplexSelector>> complexSelectorList;
    std::shared_ptr<ComplexSelector> currentComplexSelector = std::make_shared<ComplexSelector>();
    std::shared_ptr<CompoundSelector> currentCompoundSelector = std::make_shared<CompoundSelector>();
    bool typeSelectorAdded = false;
    bool isNextSelectorOfTypeClass = false;

    for (auto prelude: preludes)
    {
        if (prelude.type == CSSTokenType::Comma)
        {
            //Reset current selectors
            currentComplexSelector->m_compoundSelector = currentCompoundSelector;
            currentComplexSelector->calculateSelectorSpecifity();
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
        if (prelude.type == CSSTokenType::IdentLike && !typeSelectorAdded && !isNextSelectorOfTypeClass)
        {
            std::shared_ptr<SimpleSelector> typeSelector = std::make_shared<SimpleSelector>(prelude.value(), SelectorType::Type);
            typeSelectorAdded = true;
            currentCompoundSelector->m_typeSelector = std::move(typeSelector);
            continue;
        }
        else if (prelude.type == CSSTokenType::Delim && prelude.value() == "*" && !typeSelectorAdded && !isNextSelectorOfTypeClass)
        {
            std::shared_ptr<SimpleSelector> univerisalSelector = std::make_shared<SimpleSelector>(prelude.value(), SelectorType::Universal);
            typeSelectorAdded = true;
            currentCompoundSelector->m_typeSelector = std::move(univerisalSelector);
            continue;
        }
        else if (prelude.type == CSSTokenType::IdentLike && typeSelectorAdded && !isNextSelectorOfTypeClass)
        {
            printf("Parsing selector returned failure\n!");
            exit(0);
        }

        //<subclass-selector> = <id-selector> | <class-selector> |
        //                      <attribute-selector> | <pseudo-class-selector>
        // <id-selector> = <hash-token>
        if (prelude.type == CSSTokenType::Hash)
        {
            std::shared_ptr<SimpleSelector> idSelector = std::make_shared<SimpleSelector>(prelude.value(), SelectorType::Id);
            currentCompoundSelector->m_subClassSelectors.push_back(std::move(idSelector));
            continue;
        }

        // <class-selector> = '.' <ident-token>
        if (prelude.type == CSSTokenType::Delim && prelude.value() == ".")
        {
            isNextSelectorOfTypeClass = true;
            continue;
        }

        if (isNextSelectorOfTypeClass && prelude.type == CSSTokenType::IdentLike)
        {
            std::shared_ptr<SimpleSelector> classSelector = std::make_shared<SimpleSelector>(prelude.value(), SelectorType::Class);
            currentCompoundSelector->m_subClassSelectors.push_back(std::move(classSelector));
            isNextSelectorOfTypeClass = false;
            continue;
        }
    }

    currentComplexSelector->m_compoundSelector = std::move(currentCompoundSelector);
    currentComplexSelector->calculateSelectorSpecifity();
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
        switch (simpleBlockToken.type)
        {
            case CSSTokenType::LeftCurlyBracket:
            case CSSTokenType::Whitespace:
            case CSSTokenType::Semicolon:
                 simpleBlockTokenIndex++;
                 continue;
            //<at-keyword-token>
            case CSSTokenType::IdentLike:
                std::vector<CSSToken> tempList;

                if (simpleBlockTokenIndex < simpleBlock->values.size() &&
                        simpleBlock->values[simpleBlockTokenIndex].type != CSSTokenType::Semicolon &&
                        simpleBlock->values[simpleBlockTokenIndex].type != CSSTokenType::EndOfFile)
                {
                    tempList.push_back(simpleBlock->values[simpleBlockTokenIndex]);
                }

                while(simpleBlockTokenIndex + 1 < simpleBlock->values.size())
                {
                    if (simpleBlock->values[simpleBlockTokenIndex + 1].type != CSSTokenType::Semicolon &&
                            simpleBlock->values[simpleBlockTokenIndex + 1].type != CSSTokenType::EndOfFile)
                    {
                        tempList.push_back(simpleBlock->values[simpleBlockTokenIndex + 1]);
                        simpleBlockTokenIndex++;
                    }
                    else
                    {
                       for (auto resolvedDeclaration: resolveShorthandDeclaration(consumeDeclaration(tempList)))
                       {
                           declarations.push_back(resolvedDeclaration);
                       }
                       break;
                    }
                }

                break;
        }
    }

    return declarations;
}

std::shared_ptr<Declaration> CSS::Parser::consumeDeclaration(const std::vector<CSSToken>& tempList)
{
    std::shared_ptr<Declaration> declaration = std::make_shared<Declaration>();
    bool isDeclarationNameSet = false;
    bool isDeclarationValueNext = false;

    for(CSSToken simpleBlockToken: tempList)
    {
         // First token will always be an ident like
        if (simpleBlockToken.type == CSSTokenType::IdentLike && !isDeclarationValueNext)
        {
            declaration = std::make_shared<Declaration>();
            declaration->name = simpleBlockToken.value();
            continue;
        }

        if (simpleBlockToken.type == CSSTokenType::Whitespace)
        {
            continue;
        }

        if (simpleBlockToken.type == CSSTokenType::Colon)
        {
            isDeclarationValueNext = true;
            continue;
        }
        else if (!isDeclarationValueNext)
        {
           printf("PARSE ERROR!\n");
           exit(0);
        }

        if (simpleBlockToken.type != CSSTokenType::EndOfFile)
        {
            declaration->value.push_back(simpleBlockToken);
            continue;
        }

        // Handle !important attributes here...

        if (simpleBlockToken.type == CSSTokenType::Whitespace)
            continue;

    }

    return declaration;
}

std::vector<std::shared_ptr<Declaration>> CSS::Parser::resolveShorthandDeclaration(std::shared_ptr<Declaration> declaration)
{
    std::vector<std::shared_ptr<Declaration>> longHandDeclarations;
    if (Tools::caseInsensitiveStringCompare(declaration->name, "padding"))
    {
        for (auto resolvedLongHandDeclaration: resolvePosistionalShorthandDeclarationValues(ShortHandPropertyNameType::Padding, declaration))
        {
            longHandDeclarations.push_back(resolvedLongHandDeclaration);
        }

    }
    else if (Tools::caseInsensitiveStringCompare(declaration->name, "margin"))
    {
        for (auto resolvedLongHandDeclaration: resolvePosistionalShorthandDeclarationValues(ShortHandPropertyNameType::Margin, declaration))
        {
            longHandDeclarations.push_back(resolvedLongHandDeclaration);
        }
    }
    else if (Tools::caseInsensitiveStringCompare(declaration->name, "border"))
    {
        for (auto resolvedLongHandDeclaration: resolvePosistionalShorthandDeclarationValues(ShortHandPropertyNameType::Border, declaration))
        {
            longHandDeclarations.push_back(resolvedLongHandDeclaration);
        }
    }
    else
    {
        longHandDeclarations.push_back(declaration);
    }

    return longHandDeclarations;
}

std::shared_ptr<Declaration> CSS::Parser::createPositionalDeclaration(std::string declarationName, const CSSToken declarationValue)
{
    std::shared_ptr<Declaration> declaration = std::make_shared<Declaration>();
    declaration->name = declarationName;
    declaration->value.push_back(declarationValue);

    return declaration;
}


std::vector<std::shared_ptr<Declaration>> CSS::Parser::resolvePosistionalShorthandDeclarationValues(ShortHandPropertyNameType type, const std::shared_ptr<Declaration> declaration)
{
    std::vector<std::shared_ptr<Declaration>> resolvedDeclarations;
    int declarationValueCount = declaration->value.size();


    switch (type)
    {
        case ShortHandPropertyNameType::Margin:
            if (declarationValueCount == 1)
            {
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-top", declaration->value[0])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-bottom", declaration->value[0])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-left", declaration->value[0])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-right", declaration->value[0])));
            }
            else if (declarationValueCount == 2)
            {
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-top", declaration->value[0])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-bottom", declaration->value[0])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-left", declaration->value[1])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-right", declaration->value[1])));
            }
            else if (declarationValueCount == 3)
            {
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-top", declaration->value[0])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-bottom", declaration->value[2])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-left", declaration->value[1])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-right", declaration->value[1])));
            }
            else if (declarationValueCount == 4)
            {
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-top", declaration->value[0])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-bottom", declaration->value[2])));
                resolvedDeclarations.push_back(std::move(createPositionalDeclaration("margin-left", declaration->value[3])));
                resolvedDeclarations.push_back(createPositionalDeclaration("margin-right", declaration->value[1]));
            }
            break;

        case ShortHandPropertyNameType::Padding:
            if (declarationValueCount == 1)
            {
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-top", declaration->value[0]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-bottom", declaration->value[0]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-left", declaration->value[0]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-right", declaration->value[0]));
            }
            else if (declarationValueCount == 2)
            {
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-top", declaration->value[0]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-bottom", declaration->value[0]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-left", declaration->value[1]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-right", declaration->value[1]));
            }
            else if (declarationValueCount == 3)
            {
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-top", declaration->value[0]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-bottom", declaration->value[2]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-left", declaration->value[1]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-right", declaration->value[1]));
            }
            else if (declarationValueCount == 4)
            {
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-top", declaration->value[0]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-bottom", declaration->value[2]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-left", declaration->value[3]));
                resolvedDeclarations.push_back(createPositionalDeclaration("padding-right", declaration->value[1]));
            }
            break;
        case ShortHandPropertyNameType::Border:
            if (declarationValueCount == 3)
            {
                resolvedDeclarations.push_back(createPositionalDeclaration("border-top-width", declaration->value[0]));
                resolvedDeclarations.push_back(createPositionalDeclaration("border-bottom-width", declaration->value[0]));
                resolvedDeclarations.push_back(createPositionalDeclaration("border-left-width", declaration->value[0]));
                resolvedDeclarations.push_back(createPositionalDeclaration("border-right-width", declaration->value[0]));

                resolvedDeclarations.push_back(createPositionalDeclaration("border-top-style", declaration->value[1]));
                resolvedDeclarations.push_back(createPositionalDeclaration("border-bottom-style", declaration->value[1]));
                resolvedDeclarations.push_back(createPositionalDeclaration("border-left-style", declaration->value[1]));
                resolvedDeclarations.push_back(createPositionalDeclaration("border-right-style", declaration->value[1]));

                resolvedDeclarations.push_back(createPositionalDeclaration("border-top-color", declaration->value[2]));
                resolvedDeclarations.push_back(createPositionalDeclaration("border-bottom-color", declaration->value[2]));
                resolvedDeclarations.push_back(createPositionalDeclaration("border-left-color", declaration->value[2]));
                resolvedDeclarations.push_back(createPositionalDeclaration("border-right-color", declaration->value[2]));
            }


    }

    return resolvedDeclarations;
}


const CSSToken& CSS::Parser::peek()
{
    return tokens[currentCSSToken];
}

const CSSToken& CSS::Parser::peekNext()
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
