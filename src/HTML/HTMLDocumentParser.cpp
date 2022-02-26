#include <HTML/HTMLDocumentParser.h>
#include <HTML/Tokenizer.h>
#include <DOM/Document.h>
#include <DOM/HTMLElement.h>
#include <DOM/HTMLHeadElement.h>
#include <HTML/HTMLToken.h>
#include <DOM/Comment.h>
#include <DOM/DocumentType.h>
#include <DOM/Text.h>
#include <stack>
#include <unordered_set>
#include <HTML/Lexer.h>
#include <Logger.h>

std::string InsertionModeDictionary[] =
{
    "Initial",
    "BeforeHTML",
    "BeforeHead",
    "BeforeHeadNoScript",
    "InHead",
    "AfterHead",
    "InBody",
    "Text",
    "InTable",
    "InTableText",
    "InCaption",
    "InColumnGroup",
    "InTableBody",
    "InRow",
    "InCell",
    "InSelect",
    "InSelectInTable",
    "InTemplate",
    "AfterBody",
    "InFrameset",
    "AfterFrameset",
    "AfterAfterBody",
    "AfterAfterFrameset"
};

std::string TokenTypesDocParser[] =
{
    "Doctype",
    "StartTag",
    "EndTag",
    "Comment",
    "Character",
    "EndOfFile",

};

std::unordered_set<std::string> InBodyTagNames
{
    "address",
    "article",
    "aside",
    "blockquote",
    "center",
    "details",
    "dialog",
    "dir",
    "div",
    "dl",
    "fieldset",
    "figcaption",
    "figure",
    "footer",
    "header",
    "hgroup",
    "main",
    "menu",
    "nav",
    "ol",
    "p",
    "section",
    "summary",
    "ul"
};

std::unordered_set<std::string> InBodyHeaderTagNames
{
    "h1", "h2", "h3", "h4", "h5", "h6"
};

std::unordered_set<std::string> InBodyFormattingTags
{
    "a", "b", "big", "code", "em", "font", "i", "s", "small", "strike", "strong", "tt", "u"
};

bool containsTagName(std::unordered_set<std::string> tagNames, std::string tagName)
{
    if (tagNames.count(tagName) != 0)
    {
        return true;
    }
    return false;
}


HTMLDocumentParser::HTMLDocumentParser(std::vector<HTMLToken> tokens) : tokens(tokens)
{
    insertionMode = InsertionMode::Initial;
    m_document = std::make_unique<Document>();
    m_document->nodeName = "Document";
    m_document->nodeType = Node::NodeType::DocumentNode;
}


void HTMLDocumentParser::switchToState(InsertionMode newMode)
{
#ifdef PRINT_STATE_SWITCHING_DEBUG
    printf("Switching state from [%s] to [%s]\n", InsertionModeDictionary[insertionMode].c_str(), InsertionModeDictionary[newMode].c_str());
#endif
    insertionMode = newMode;
}


void ParseError()
{
    printf("%s\n", "Parse Error");
    exit(0);
}

void Todo()
{
    printf("%s\n", "This is a todo!");
    exit(0);
}

std::weak_ptr<HTMLElement> HTMLDocumentParser::findApproriatePlaceForInsertingNode()
{
    // Foster Parenting?

    return openElements.top();
}

void HTMLDocumentParser::insertHTMLElementNode(const HTMLToken& token, std::weak_ptr<Node> parentNode)
{
    std::shared_ptr<HTMLElement> htmlElement = std::make_shared<HTMLElement>(parentNode);
    htmlElement->setTagName(token.getTagName());
    htmlElement->nodeName = token.getTagName();
    htmlElement->nodeType = Node::NodeType::ElementNode;
    htmlElement->textContent = htmlElement->getTextContent();
    htmlElement->ownerDocument = m_document;

    // TODO: Add previous and next sibling assignment.

    for (auto attr: token.tag.attributes)
        htmlElement->attributes.insert(std::pair<std::string, std::string>(attr.name, attr.value));

    if (htmlElement->attributes.find("id") != htmlElement->attributes.end())
    {
        htmlElement->id = htmlElement->attributes.find("id")->second.c_str();
    }

    // Multiple references are needed here (possibly weak_ptr the openElements?)

    openElements.push(htmlElement);
    htmlElement->parentNode.lock()->childNodes.push_back(std::move(htmlElement));
}

void HTMLDocumentParser::insertTextNode(const HTMLToken& token)
{
    if (auto documentNode = dynamic_cast<Document*>(findApproriatePlaceForInsertingNode().lock().get()))
    {
        return;
    }

    auto nodeInsertLocation = findApproriatePlaceForInsertingNode();

    if (nodeInsertLocation.lock()->childNodes.size() > 0 && nodeInsertLocation.lock()->childNodes[nodeInsertLocation.lock()->childNodes.size() - 1]->nodeType == Node::NodeType::TextNode)
    {
        if (auto textNode = dynamic_cast<class Text*>(nodeInsertLocation.lock()->childNodes[nodeInsertLocation.lock()->childNodes.size() - 1].get()))
        {
            textNode->appendData(token.getCommentOrCharacterData());
            nodeInsertLocation.lock()->childNodes[nodeInsertLocation.lock()->childNodes.size() - 1]->nodeValue += token.getCommentOrCharacterData();
            return;
        }
    }

    std::shared_ptr<class Text> text = std::make_shared<class Text>(findApproriatePlaceForInsertingNode().lock());
    text->data = token.getCommentOrCharacterData();
    text->nodeName = "Text";
    text->nodeType = Node::NodeType::TextNode;
    text->nodeValue = token.getCommentOrCharacterData();
    text->ownerDocument = m_document;
    nodeInsertLocation.lock()->childNodes.push_back(std::move(text));
}

void HTMLDocumentParser::processInitial(const HTMLToken& token)
{
    if (token.isCharacterToken() && token.isParserWhitespace())
    {
        return;
    }

    if (token.isCommentToken())
    {
        std::shared_ptr<Comment> comment = std::make_shared<Comment>(m_document, token.getCommentOrCharacterData());
        m_document->childNodes.push_back(std::move(comment));
        return;
    }

    if (token.isDoctypeToken())
    {

        if (token.getDoctypeName() != "html" |
            !token.getPublicIdentifier().empty() |
            !token.getSystemIdentifier().empty() |
            token.getSystemIdentifier() == "about:legacy-compat")
        {
            ParseError();
        }
        else
        {
            std::shared_ptr<DocumentType> documentType = std::make_shared<DocumentType>(m_document);
            documentType->setName(token.getDoctypeName());
            m_document->childNodes.push_back(std::move(documentType));
            switchToState(InsertionMode::BeforeHTML);
            return;
        }
    }

    switchToState(InsertionMode::BeforeHTML);
    return;
}

void HTMLDocumentParser::processBeforeHTML(const HTMLToken& token)
{
    if (token.isCharacterToken() && token.isParserWhitespace())
    {
        return;
    }

    if (token.isDoctypeToken())
    {
        ParseError();
        exit(0);
    }

    if (token.isCommentToken())
    {
        std::shared_ptr<Comment> comment = std::make_shared<Comment>(m_document, token.getCommentOrCharacterData());
        m_document->childNodes.push_back(std::move(comment));
        return;
    }

    if (token.isStartTagToken() && token.getTagName() == "html")
    {
        insertHTMLElementNode(token, m_document);
        switchToState(InsertionMode::BeforeHead);
        return;
    }

    if ((token.isEndTagToken() && token.getTagName() == "head") |
             (token.isEndTagToken() && token.getTagName() == "body") |
             (token.isEndTagToken() && token.getTagName() == "html") |
            (token.isEndTagToken() && token.getTagName() == "br"))
    {
        std::shared_ptr<Comment> comment = std::make_shared<Comment>(m_document, token.getCommentOrCharacterData());
        m_document->childNodes.push_back(std::move(comment));
        switchToState(InsertionMode::BeforeHead);
        return;
    }

    ParseError();
}

void HTMLDocumentParser::processBeforeHead(const HTMLToken& token)
{
    if (token.isCharacterToken() && token.isParserWhitespace())
    {
        return;
    }

    if (token.isCommentToken())
    {
        std::shared_ptr<Comment> comment = std::make_shared<Comment>(findApproriatePlaceForInsertingNode(), token.getCommentOrCharacterData());
        m_document->childNodes.push_back(std::move(comment));
        return;
    }

    if (token.isDoctypeToken())
    {
        ParseError();
    }

    if (token.isStartTagToken() && token.getTagName() == "html")
    {
        switchToState(InsertionMode::BeforeHTML);
        return;
    }

    if (token.isStartTagToken() && token.getTagName() == "head")
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        switchToState(InsertionMode::InHead);
        return;
    }
}

void HTMLDocumentParser::processInHead(const HTMLToken& token)
{
    if (token.isCharacterToken() && token.isParserWhitespace())
    {
        insertTextNode(token);
        return;
    }

    if (token.isCommentToken())
    {
        std::shared_ptr<Comment> comment = std::make_shared<Comment>(findApproriatePlaceForInsertingNode(), token.getCommentOrCharacterData());
        findApproriatePlaceForInsertingNode().lock()->childNodes.push_back(std::move(comment));
        return;
    }

    if (token.isDoctypeToken())
    {
        ParseError();
    }

    if (token.isEndTagToken() && token.getTagName() == "head")
    {
        openElements.pop();
        switchToState(InsertionMode::AfterHead);
        return;
    }

    if (token.isStartTagToken() && token.getTagName() == "title")
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        switchToState(InsertionMode::Text);
        return;
    }

    if (token.isStartTagToken() && token.getTagName() == "style")
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        switchToState(InsertionMode::Text);
        return;
    }
}

void HTMLDocumentParser::processAfterHead(const HTMLToken& token)
{
    if (token.isCharacterToken() && token.isParserWhitespace())
    {
        insertTextNode(token);
        return;
    }

    if (token.isCommentToken())
    {
        std::shared_ptr<Comment> comment = std::make_shared<Comment>(findApproriatePlaceForInsertingNode(), token.getCommentOrCharacterData());
        findApproriatePlaceForInsertingNode().lock()->childNodes.push_back(std::move(comment));
        return;
    }

    if (token.isDoctypeToken())
    {
        ParseError();
    }

    if (token.isStartTagToken() && token.getTagName() == "body")
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        switchToState(InsertionMode::InBody);
        //TODO: Set frame-set flag to false
        return;
    }
}

void HTMLDocumentParser::processInBody(const HTMLToken& token)
{
    //TODO: Reconstruct the active formatting elements.
    if (token.isCharacterToken() && token.isParserWhitespace())
    {
        insertTextNode(token);
        return;
    }
    else if (token.isCharacterToken())
    {
        //TODO: Reconstruct the active formatting elements.
        insertTextNode(token);
        return;
        //TODO: Set frame-set flag to false
    }

    if (token.isCommentToken())
    {
        std::shared_ptr<Comment> comment = std::make_shared<Comment>(findApproriatePlaceForInsertingNode(), token.getCommentOrCharacterData());
        findApproriatePlaceForInsertingNode().lock()->childNodes.push_back(std::move(comment));
        return;
    }

    if (token.isDoctypeToken())
    {
        printf("1\n");
        ParseError();
    }

    if (token.isStartTagToken() && token.getTagName() == "html")
    {
        printf("2\n");
        ParseError();
    }


    if (token.isStartTagToken() && token.getTagName() == "body")
    {
        printf("3\n");
        ParseError();
    }

    if (token.isEndTagToken() && token.getTagName() == "body")
    {
        if (findApproriatePlaceForInsertingNode().lock()->tagName() != token.getTagName())
        {
            printf("4\n");
            ParseError();
        }
        else
        {
            openElements.pop();
            switchToState(InsertionMode::AfterBody);
            return;
        }
    }

    if (token.isStartTagToken() && containsTagName(InBodyTagNames, token.getTagName()))
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token.isEndTagToken() && containsTagName(InBodyTagNames, token.getTagName()))
    {
        if (findApproriatePlaceForInsertingNode().lock()->tagName() != token.getTagName())
        {
            ParseError();
        }
        else
        {
            openElements.pop();
            return;
        }
    }

    if (token.isStartTagToken() && Tools::caseInsensitiveStringCompare(token.getTagName(), "br"))
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        openElements.pop();
        return;
    }

    if (token.isStartTagToken() && containsTagName(InBodyHeaderTagNames, token.getTagName()))
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token.isEndTagToken() && containsTagName(InBodyHeaderTagNames, token.getTagName()))
    {
        //printf("find: %s\n", findApproriatePlaceForInsertingNode()->tagName.c_str());
        if (findApproriatePlaceForInsertingNode().lock()->tagName()!= token.getTagName())
        {
            ParseError();
        }
        else
        {
            openElements.pop();
            return;
        }
    }

    if (token.isStartTagToken() && containsTagName(InBodyFormattingTags, token.getTagName()))
    {
        //TODO: Push onto the list of active formatting elements that element.
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token.isEndTagToken() && containsTagName(InBodyFormattingTags, token.getTagName()))
    {
        if (findApproriatePlaceForInsertingNode().lock()->tagName() != token.getTagName())
        {
            printf("7\n");
            ParseError();
        }
        else
        {
            openElements.pop();
            return;
        }
    }


}

void HTMLDocumentParser::processAfterBody(const HTMLToken& token)
{
    if (token.isCharacterToken() && token.isParserWhitespace())
    {
        //switchToState(InsertionMode::InBody);
        return;
    }

    if (token.isCommentToken())
    {
        std::shared_ptr<Comment> comment = std::make_shared<Comment>(findApproriatePlaceForInsertingNode(), token.getCommentOrCharacterData());
        findApproriatePlaceForInsertingNode().lock()->childNodes.push_back(std::move(comment));
        return;
    }

    if (token.isDoctypeToken())
    {
        ParseError();
    }

    if (token.isEndTagToken() && token.getTagName() == "html")
    {
        return;
    }
}

void HTMLDocumentParser::processText(const HTMLToken& token)
{
    if (token.isCharacterToken())
    {
        insertTextNode(token);
        return;
    }
    else
    {
        openElements.pop();

        // TODO: Make this use the original insertion mode instead of a pre defined one!
        switchToState(InsertionMode::InHead);
        return;
    }
}


void HTMLDocumentParser::run()
{
    for (auto token: tokens)
    {
        //printf("\nCurrent Token: \n	Type: %s\n Tag Name: %s\n", TokenTypesDocParser[token.getType()].c_str(), token.getTagName().c_str());

        switch (insertionMode) {
            case Initial:
                processInitial(token);
                break;
            case BeforeHTML:
                processBeforeHTML(token);
                break;
            case BeforeHead:
                processBeforeHead(token);
                break;
            case InHead:
                processInHead(token);
                break;
            case AfterHead:
                processAfterHead(token);
                break;
            case InBody:
                processInBody(token);
                break;
            case AfterBody:
                processAfterBody(token);
                break;
            case Text:
                processText(token);
                break;
        }

    }
}
