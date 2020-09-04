#include <Parser/CSSParser.h>
#include <Parser/Lexer.h>
#include <memory>
#include <DOM/Text.h>
int cssTokenIndex;


enum ParserState
{
    Free,
    InSelectorName,
    AfterSelectorName,
    InSelector
};
std::string ParserStateTypes[] =
{
    "Free",
    "InSelectorName",
    "AfterSelector",
    "InSelector"
};

ParserState cssTokenizerState;


CSSParser::CSSParser(std::shared_ptr<Document> document) : document(document)
{
    cssTokenizerState = ParserState::Free;
}

void switchToState(ParserState newState)
{
    //printf("Switching state from [%s] to [%s]\n", ParserStateTypes[parserState].c_str(), ParserStateTypes[newState].c_str());
    cssTokenizerState = newState;
}

bool cssCompareChar(char &c1, char &c2)
{
    if(c1 == c2)
    {
        return true;
    }
    else if (std::toupper(c1) == std::toupper(c2))
    {
        return true;
    }

    return false;
}


bool CSSParser::caseInsensitiveStringCompare(std::string val1, std::string val2)
{
    return val1.size() == val2.size() && std::equal(val1.begin(), val1.end(), val2.begin(), &cssCompareChar);
}

char CSSParser::peek()
{
    return css.at(cssTokenIndex);
}

char CSSParser::consume()
{
    return css.at(cssTokenIndex++);
}

void CSSParser::consumeWhitespace()
{
    for (int i = cssTokenIndex; i < css.length(); i++)
    {
        if (peek() == ' ' || peek() == '\n')
        {
            cssTokenIndex++;
        }
        else
        {
            break;
        }
    }
}

std::string CSSParser::consumeUntilChar(char tokenData)
{
    std::string returnString;
    for (int i = cssTokenIndex; i < css.length(); i++)
    {
        consumeWhitespace();

        if (peek() != tokenData)
        {
            returnString += peek();
        }
        else
        {
            return returnString;
        }
        cssTokenIndex++;

    }

    return returnString;
}

void CSSParser::runCSSContent(std::string cssContent)
{
    this->css = cssContent;
    styleSheet = std::make_shared<Stylesheet>();

    printf("CSS: \n%s\n", css.c_str());

    std::shared_ptr<StyleRule> styleRule;

    std::string selectorNameBuffer;
    std::string selectorPropertyNameBuffer;
    std::string selectorPropertyValueBuffer;

    while (cssTokenIndex < css.length())
    {

        switch (cssTokenizerState) {
            case ParserState::Free:
                consumeWhitespace();

                if (cssTokenIndex == css.length())
                {
                    break;
                }

                switchToState(ParserState::InSelectorName);
                break;
            case ParserState::InSelectorName:

                consumeWhitespace();

                if (peek() == '{')
                {
                    styleRule = std::make_shared<StyleRule>(selectorNameBuffer);
                    switchToState(ParserState::AfterSelectorName);
                    break;
                }
                else
                {
                    selectorNameBuffer += consume();
                    break;
                }
            case ParserState::AfterSelectorName:
                consumeWhitespace();
                consume(); // {
                switchToState(ParserState::InSelector);
                break;

            case ParserState::InSelector:
                consumeWhitespace();

                if (peek() == '}')
                {
                    consume();

                    styleSheet->styles.push_back(styleRule);
                    switchToState(ParserState::Free);
                    break;
                }
                else
                {
                    selectorPropertyNameBuffer.append(consumeUntilChar(':'));
                    consume();
                    selectorPropertyValueBuffer.append(consumeUntilChar(';'));
                    consume();

                    styleRule->styleMap->insert(std::make_pair(selectorPropertyNameBuffer, selectorPropertyValueBuffer));

                    selectorNameBuffer.clear();
                    selectorPropertyNameBuffer.clear();
                    selectorPropertyValueBuffer.clear();

                    break;
                }
        }
    }


    runSelectorEngine();

    printRenderTree();
}

void CSSParser::printNode(std::shared_ptr<Node> node, std::string indent, bool last)
{
    printf("%s%s\033[1;32m%d\033[0m\n", indent.c_str(), "\033[1;32m+- \033[0m", node->nodeType);
    printf("%s%s%s\n", indent.c_str(), " Node Name: ", node->nodeName.c_str());
    printf("%s%s%s\n", indent.c_str(), " Node Value: ", node->nodeValue.c_str());
    printf("%s%s%s\n", indent.c_str(), " Base URI: ", node->baseURI.c_str());
    printf("%s%s%d\n", indent.c_str(), " Is Connected: ", node->isConnected);
    printf("%s%s%s\n", indent.c_str(), " Text Content: ", node->getTextContent().c_str());

    std::string returnStr;
    //printf("RenderNode ref count: %ld\n", node->renderNode.use_count());

    if (node->css && node->css->styleRules->styleMap->size() > 0)
    {
        printf("%sStyle [%s]: \n", indent.c_str(), node->css->styleRules->selector.c_str());
        for (auto stylePropVal: *node->css->styleRules->styleMap)
        {
            returnStr += "	{" + stylePropVal.first + ", " + stylePropVal.second + "}\n";
        }
        printf("%s%s", indent.c_str(), returnStr.c_str());
    }

    if (auto commentNode = dynamic_cast<Comment*>(node.get()))
    {
        printf("%s%s%s\n", indent.c_str(), " Data: ", commentNode->data.c_str());
        printf("%s%s%d\n", indent.c_str(), " Length: ", commentNode->length);
    }

    if (auto documentTypeNode = dynamic_cast<DocumentType*>(node.get()))
    {
        printf("%s%s%s\n", indent.c_str(), " Name: ", documentTypeNode->name.c_str());
        printf("%s%s%s\n", indent.c_str(), " Public ID: ", documentTypeNode->publicId.c_str());
        printf("%s%s%s\n", indent.c_str(), " System ID: ", documentTypeNode->systemId.c_str());
    }

    if (auto elementNode = dynamic_cast<HTMLElement*>(node.get()))
    {
        printf("%s%s%s\n", indent.c_str(), " Namespace URI: ", elementNode->namespaceURI.c_str());
        printf("%s%s%s\n", indent.c_str(), " Prefix: ", elementNode->prefix.c_str());
        printf("%s%s%s\n", indent.c_str(), " Local Name: ", elementNode->localName.c_str());
        printf("%s%s%s\n", indent.c_str(), " Tag Name: ", elementNode->tagName.c_str());
        if (elementNode->hasAttributes())
        {
            printf("%s%s\n", indent.c_str(), " Attributes: ");
            for (auto &attribute: *elementNode->attributes)
            {
                printf("	%s%s=%s\n", indent.c_str(), attribute.first.c_str(), attribute.second.c_str());
            }
        }
    }

    if (auto textNode = dynamic_cast<class Text*>(node.get()))
    {
        printf("%s%s%s\n", indent.c_str(), " Data: ", textNode->data.c_str());
        printf("%s%s%d\n", indent.c_str(), " Length: ", textNode->length);
    }


    // Owner Document
    // Parent Node
    // Child Nodes

    indent += last ? " " : "|  ";

    for (int i = 0; i < node->childNodes.size(); i++)
    {
        printNode(node->childNodes[i], indent, i == node->childNodes.size() - 1);
    }
}

bool CSSParser::isJustWhiteSpace(std::shared_ptr<Node> node)
{
    bool isJustWhiteSpace = false;
    std::string textContent = node->getTextContent();
    for (int i = 0; i < textContent.length(); i++)
    {
        if (textContent.at(i) == ' ' || textContent.at(i) == '\n' || textContent.at(i) == '\t')
        {
            isJustWhiteSpace = true;
        }
        else
        {
            return false;
        }
    }

    return isJustWhiteSpace;
}

void CSSParser::printRenderTree()
{

    printf("\n------------------Render Tree [%ld]: ------------------\n\n", document->childNodes.size());
    printNode(document, "", false);

}

void CSSParser::applyAllRule(std::shared_ptr<Node> node, std::string type, std::shared_ptr<StyleRule> styleRules)
{
    std::shared_ptr<Node> returnNode;

    if (node->nodeName.compare("Text") == 0  || node->nodeName.compare("Document Type") == 0  ||  node->nodeName.compare("head") == 0 || node->nodeName.compare("Comment") == 0  || node->nodeName.compare("title") == 0 || node->nodeName.compare("style") == 0 || node->nodeName.compare("script") == 0)
        return;

    if (node->css.get() != nullptr && node->css->styleRules.get() != nullptr)
    {
        for (auto stylePropVal: *node->css->styleRules->styleMap)
        {
            for (auto cssStyleRule: *styleRules->styleMap)
            {
                if (cssStyleRule.first.compare(stylePropVal.first) != 0)
                {
                    printf("INSERTING ALL : %s, %s\n", cssStyleRule.first.c_str(), cssStyleRule.second.c_str());
                    node->css->styleRules->styleMap->insert(std::make_pair(cssStyleRule.first, cssStyleRule.second));
                }
            }
        }
    }
    else
    {
        printf("NEWING STYLERULES!\n");
        node->css = std::make_shared<RenderNode>(type);
        node->css->styleRules->selector = styleRules->selector;
        node->css->styleRules = styleRules;
    }


    for (int i = 0; i < node->childNodes.size(); i++)
    {
        applyAllRule(node->childNodes[i], type, styleRules);
    }

}

void CSSParser::applyTypeRule(std::shared_ptr<Node> node, std::string type, std::shared_ptr<StyleRule> styleRules)
{
    std::shared_ptr<Node> returnNode;

    if (node->nodeName.compare("Text") == 0 || node->nodeName.compare("Document Type") == 0  ||  node->nodeName.compare("head") == 0 || node->nodeName.compare("Comment") == 0  || node->nodeName.compare("title") == 0 || node->nodeName.compare("style") == 0 || node->nodeName.compare("script") == 0)
        return;

    if (caseInsensitiveStringCompare(type, node->nodeName))
    {
        printf("Style Rule Selector: %s, Node Name: %s\n", styleRules->selector.c_str(), node->nodeName.c_str());
        if (node->css.get() != nullptr && node->css->styleRules.get() != nullptr && node->css->styleRules->styleMap != nullptr)
        {
            for (auto stylePropVal: *node->css->styleRules->styleMap)
            {

                for (auto cssStyleRule: *styleRules->styleMap)
                {
                    printf("Comparing Style: %s, %s With %s, %s\n\n", stylePropVal.first.c_str(), stylePropVal.second.c_str(),  cssStyleRule.first.c_str(), cssStyleRule.second.c_str());
                    if (cssStyleRule.first.compare(stylePropVal.first) == 0)
                    {

                         //node->css->styleRules->styleMap->emplace(std::make_pair(cssStyleRule.first, cssStyleRule.second));
                         //node->css->styleRules->selector = type;

                        printf("Override Rule: %s, %s With %s, %s\n", stylePropVal.first.c_str(), stylePropVal.second.c_str(), cssStyleRule.first.c_str(), cssStyleRule.second.c_str());
                        node->css->styleRules->styleMap->erase(node->css->styleRules->styleMap->find(cssStyleRule.first));

                        node->css->styleRules->styleMap->insert(std::make_pair(cssStyleRule.first, cssStyleRule.second));
                        node->css->styleRules->selector = node->nodeName;
                    }
                    else
                    {
                        printf("Inserting new Rule!\n");
                        //node->css->styleRules->selector = styleRules->selector;
                        node->css->styleRules->styleMap->insert(std::make_pair(cssStyleRule.first, cssStyleRule.second));
                    }

                }
            }
        }
        else
        {
            node->css = std::make_shared<RenderNode>(type);
            node->css->styleRules->selector = styleRules->selector;
            node->css->styleRules = styleRules;
        }
    }

    for (int i = 0; i < node->childNodes.size(); i++)
    {
        applyTypeRule(node->childNodes[i], type, styleRules);
    }

}

void CSSParser::runSelectorEngine()
{
    std::vector<std::shared_ptr<StyleRule>> queuedStyles;
    //for (auto style: styleSheet->styles)
    //{
    //	if (style->selector.at(0) == '*')
    //	{
    //		printf("All Rule Style Selector: %s\n", style->selector.c_str());
    //		applyAllRule(document, style->selector, style);
    //	}
    //	else
    //	{
    //		queuedStyles.push_back(style);
    //	}
    //}

    for (auto style: styleSheet->styles)
    {
        if (style->selector.at(0) == '*')
        {
            applyAllRule(document, style->selector, style);
        }
        else if (style->selector.at(0) == '.')
        {

        }
        else if (style->selector.at(0) == '#')
        {

        }
        else if (style->selector.at(0) == '[')
        {

        }
        else
        {
            printf("Type Rule Style Selector: %s\n", style->selector.c_str());
            applyTypeRule(document, style->selector, style);
        }
    }

}



