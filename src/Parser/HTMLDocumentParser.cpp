#include <Parser/HTMLDocumentParser.h>
#include <Parser/HTMLStateMachine.h>
#include <DOM/Document.h>
#include <DOM/HTMLElement.h>
#include <DOM/HTMLHeadElement.h>
#include <Parser/HTMLToken.h>
#include <DOM/Comment.h>
#include <DOM/DocumentType.h>
#include <DOM/Text.h>
#include <stack>
#include <unordered_set>
#include <Parser/Lexer.h>

enum InsertionMode
{
    Initial,
    BeforeHTML,
    BeforeHead,
    BeforeHeadNoScript,
    InHead,
    AfterHead,
    InBody,
    Text,
    InTable,
    InTableText,
    InCaption,
    InColumnGroup,
    InTableBody,
    InRow,
    InCell,
    InSelect,
    InSelectInTable,
    InTemplate,
    AfterBody,
    InFrameset,
    AfterFrameset,
    AfterAfterBody,
    AfterAfterFrameset
};

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

std::string NodeTypes[] =
{
    "ElementNode",
    "Attribute_Node",
    "TextNode",
    "CDATASectionNode",
    "EntityReferenceNode",// Historical
    "EntityNode", // Historical
    "ProcessingInstructionNode",
    "CommentNode",
    "DocumentNode",
    "DocumentTypeNode",
    "DoucmentFragmentNode",
    "NotationNode" // Historical

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

std::stack<std::shared_ptr<HTMLElement>> openElements;

InsertionMode insertionMode;


bool containsTagName(std::unordered_set<std::string> tagNames, std::string tagName)
{
    if (tagNames.count(tagName) != 0)
    {
        return true;
    }
    return false;
}


HTMLDocumentParser::HTMLDocumentParser(std::string html, std::vector<std::shared_ptr<HTMLToken>> tokens) : tokens(tokens)
{
    lexer = std::make_shared<Lexer>();
    lexer->loadContent(html);

    insertionMode = InsertionMode::Initial;
    document = std::make_shared<Document>();
    //headElement = std::make_shared<HTMLHeadElement>(nullptr, "");
}


void switchToState(InsertionMode newMode)
{
    printf("Switching state from [%s] to [%s]\n", InsertionModeDictionary[insertionMode].c_str(), InsertionModeDictionary[newMode].c_str());
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

std::shared_ptr<HTMLElement> HTMLDocumentParser::findApproriatePlaceForInsertingNode()
{
    // Foster Parenting?
    return openElements.top();
}

std::string lookupNodeTypeFromDictionary(Node::NodeType nodeType)
{
    return NodeTypes[nodeType - 1];
}

void HTMLDocumentParser::insertCommentNode(std::shared_ptr<HTMLToken> token, std::shared_ptr<Node> parentNode)
{
    std::shared_ptr<Comment> comment = std::make_shared<Comment>(parentNode);
    comment->parentNode = parentNode;
    comment->data = token->getCommentOrCharacterData();
    comment->nodeName = "Comment";
    comment->nodeType = Node::NodeType::CommentNode;
    comment->ownerDocument = document;
    //parentNode->childNodes.push_back(std::move(comment));
}

void HTMLDocumentParser::insertDocumentTypeNode(std::shared_ptr<HTMLToken> token, std::shared_ptr<Node> parentNode)
{
    std::unique_ptr<DocumentType> documentType = std::make_unique<DocumentType>(parentNode);
    documentType->parentNode = parentNode;
    documentType->name = token->getDoctypeName();
    documentType->nodeName = "Document Type";
    documentType->nodeType = Node::NodeType::DocumentTypeNode;
    documentType->ownerDocument = document;
    //parentNode->childNodes.push_back(std::move(documentType));
}

void HTMLDocumentParser::insertHTMLElementNode(std::shared_ptr<HTMLToken> token, std::shared_ptr<Node> parentNode)
{
    std::shared_ptr<HTMLElement> htmlElement = std::make_shared<HTMLElement>(parentNode);
    htmlElement->parentNode = parentNode;
    htmlElement->tagName = token->getTagName();
    htmlElement->nodeName = token->getTagName();
    htmlElement->nodeType = Node::NodeType::ElementNode;
    htmlElement->textContent = htmlElement->getTextContent();
    htmlElement->ownerDocument = document;

    if (parentNode->childNodes.size() > 0)
        htmlElement->nextSibling = parentNode->childNodes[parentNode->childNodes.size() - 1];

    if (token->getAttributes() != nullptr)
        htmlElement->attributes = token->getAttributes();

    parentNode->childNodes.push_back(htmlElement);

    openElements.push(htmlElement);
}

void HTMLDocumentParser::insertTextNode(std::shared_ptr<HTMLToken> token, std::shared_ptr<Node> parentNode)
{
    if (auto documentNode = dynamic_cast<Document*>(parentNode.get()))
    {
        return;
    }

    auto nodeInsertLocation = findApproriatePlaceForInsertingNode();

    if (nodeInsertLocation->childNodes.size() > 0 && nodeInsertLocation->childNodes[nodeInsertLocation->childNodes.size() - 1]->nodeType == Node::NodeType::TextNode)
    {
        if (auto textNode = dynamic_cast<class Text*>(nodeInsertLocation->childNodes[nodeInsertLocation->childNodes.size() - 1].get()))
        {
            textNode->appendData(token->getCommentOrCharacterData());
            nodeInsertLocation->childNodes[nodeInsertLocation->childNodes.size() - 1]->nodeValue += token->getCommentOrCharacterData();
            return;
        }
    }

    std::shared_ptr<class Text> text = std::make_shared<class Text>(findApproriatePlaceForInsertingNode());
    text->data = token->getCommentOrCharacterData();
    text->nodeName = "Text";
    text->nodeType = Node::NodeType::TextNode;
    text->nodeValue = token->getCommentOrCharacterData();
    text->ownerDocument = document;
    nodeInsertLocation->childNodes.push_back(std::move(text));
}

std::shared_ptr<Node> HTMLDocumentParser::findElementByTagName(std::shared_ptr<Node> node, std::string tagName)
{
    std::shared_ptr<Node> returnNode;
    if (node != nullptr && lexer->caseInsensitiveStringCompare(tagName, node->nodeName))
    {
        return node;
    }
    else
    {
        for (int i = 0; i < node->childNodes.size(); i++)
        {
            if (returnNode == nullptr)
                returnNode = findElementByTagName(node->childNodes[i], tagName);
        }
    }

    return returnNode;
}

void dumpTree(std::shared_ptr<Node> node, std::string indent, bool last)
{
    printf("%s%s\033[1;32m%s\033[0m\n", indent.c_str(), "\033[1;32m+- \033[0m", lookupNodeTypeFromDictionary(node->nodeType).c_str());
    printf("%s%s%s\n", indent.c_str(), " Node Name: ", node->nodeName.c_str());
    printf("%s%s%s\n", indent.c_str(), " Node Value: ", node->nodeValue.c_str());
    printf("%s%s%s\n", indent.c_str(), " Base URI: ", node->baseURI.c_str());
    printf("%s%s%d\n", indent.c_str(), " Is Connected: ", node->isConnected);
    printf("%s%s%s\n", indent.c_str(), " Text Content: ", node->getTextContent().c_str());

    if (node->nextSibling)
        printf("%s%s%s\n", indent.c_str(), " Sibling Node Name: ", node->nextSibling->nodeName.c_str());

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

        dumpTree(node->childNodes[i], indent, i == node->childNodes.size() - 1);
    }
}

void HTMLDocumentParser::printDOMTree()
{

    printf("\n------------------DOM Tree [%ld]: ------------------\n\n", document->childNodes.size());

    for (int i = 0; i < document->childNodes.size(); i++)
    {
        dumpTree(document->childNodes[i], "", false);
    }

}

void HTMLDocumentParser::processInitial(std::shared_ptr<HTMLToken> token)
{
    if (token->isCharacterToken() && token->isParserWhitespace())
    {
        return;
    }

    if (token->isCommentToken())
    {
        insertCommentNode(token, document);
        return;
    }

    if (token->isDoctypeToken())
    {

        if (token->getDoctypeName() != "html" |
            !token->getPublicIdentifier().empty() |
            !token->getSystemIdentifier().empty() |
            token->getSystemIdentifier() == "about:legacy-compat")
        {
            ParseError();
        }
        else
        {
            insertDocumentTypeNode(token, document);
            switchToState(InsertionMode::BeforeHTML);
            return;
        }
    }

    switchToState(InsertionMode::BeforeHTML);
    return;
}

void HTMLDocumentParser::processBeforeHTML(std::shared_ptr<HTMLToken> token)
{
    if (token->isCharacterToken() && token->isParserWhitespace())
    {
        return;
    }

    if (token->isDoctypeToken())
    {
        ParseError();
        exit(0);
    }

    if (token->isCommentToken())
    {
        insertCommentNode(token, document);
        return;
    }

    if (token->isStartTagToken() && token->getTagName() == "html")
    {
        insertHTMLElementNode(token, document);
        switchToState(InsertionMode::BeforeHead);
        return;
    }

    if ((token->isEndTagToken() && token->getTagName() == "head") |
             (token->isEndTagToken() && token->getTagName() == "body") |
             (token->isEndTagToken() && token->getTagName() == "html") |
            (token->isEndTagToken() && token->getTagName() == "br"))
    {
        insertHTMLElementNode(token, document);
        switchToState(InsertionMode::BeforeHead);
        return;
    }

    ParseError();
}

void HTMLDocumentParser::processBeforeHead(std::shared_ptr<HTMLToken> token)
{
    if (token->isCharacterToken() && token->isParserWhitespace())
    {
        return;
    }

    if (token->isCommentToken())
    {
        insertCommentNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token->isDoctypeToken())
    {
        ParseError();
    }

    if (token->isStartTagToken() && token->getTagName() == "html")
    {
        switchToState(InsertionMode::BeforeHTML);
        return;
    }

    if (token->isStartTagToken() && token->getTagName() == "head")
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        switchToState(InsertionMode::InHead);
        return;
    }
}

void HTMLDocumentParser::processInHead(std::shared_ptr<HTMLToken> token)
{
    if (token->isCharacterToken() && token->isParserWhitespace())
    {
        insertTextNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token->isCommentToken())
    {
        insertCommentNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token->isDoctypeToken())
    {
        ParseError();
    }

    if (token->isEndTagToken() && token->getTagName() == "head")
    {
        openElements.pop();
        switchToState(InsertionMode::AfterHead);
        return;
    }

    if (token->isStartTagToken() && token->getTagName() == "title")
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        switchToState(InsertionMode::Text);
        return;
    }

    if (token->isStartTagToken() && token->getTagName() == "style")
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        switchToState(InsertionMode::Text);
        return;
    }
}

void HTMLDocumentParser::processAfterHead(std::shared_ptr<HTMLToken> token)
{
    if (token->isCharacterToken() && token->isParserWhitespace())
    {
        insertTextNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token->isCommentToken())
    {
        insertCommentNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token->isDoctypeToken())
    {
        ParseError();
    }

    if (token->isStartTagToken() && token->getTagName() == "body")
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        switchToState(InsertionMode::InBody);
        //TODO: Set frame-set flag to false
        return;
    }
}

void HTMLDocumentParser::processInBody(std::shared_ptr<HTMLToken> token)
{
    //TODO: Reconstruct the active formatting elements.
    if (token->isCharacterToken() && token->isParserWhitespace())
    {
        insertTextNode(token, findApproriatePlaceForInsertingNode());
        return;
    }
    else if (token->isCharacterToken())
    {
        //TODO: Reconstruct the active formatting elements.
        insertTextNode(token, findApproriatePlaceForInsertingNode());
        return;
        //TODO: Set frame-set flag to false
    }

    if (token->isCommentToken())
    {
        insertCommentNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token->isDoctypeToken())
    {
        printf("1\n");
        ParseError();
    }

    if (token->isStartTagToken() && token->getTagName() == "html")
    {
        printf("2\n");
        ParseError();
    }


    if (token->isStartTagToken() && token->getTagName() == "body")
    {
        printf("3\n");
        ParseError();
    }

    if (token->isEndTagToken() && token->getTagName() == "body")
    {
        if (findApproriatePlaceForInsertingNode()->tagName != token->getTagName())
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

    if (token->isStartTagToken() && containsTagName(InBodyTagNames, token->getTagName()))
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token->isEndTagToken() && containsTagName(InBodyTagNames, token->getTagName()))
    {
        if (findApproriatePlaceForInsertingNode()->tagName != token->getTagName())
        {
            printf("5\n");
            ParseError();
        }
        else
        {
            openElements.pop();
            return;
        }
    }

    if (token->isStartTagToken() && lexer->caseInsensitiveStringCompare(token->getTagName(), "br"))
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        openElements.pop();
        return;
    }

    if (token->isStartTagToken() && containsTagName(InBodyHeaderTagNames, token->getTagName()))
    {
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token->isEndTagToken() && containsTagName(InBodyHeaderTagNames, token->getTagName()))
    {
        //printf("find: %s\n", findApproriatePlaceForInsertingNode()->tagName.c_str());
        if (findApproriatePlaceForInsertingNode()->tagName != token->getTagName())
        {
            printf("6\n");
            ParseError();
        }
        else
        {
            openElements.pop();
            return;
        }
    }

    if (token->isStartTagToken() && containsTagName(InBodyFormattingTags, token->getTagName()))
    {
        //TODO: Push onto the list of active formatting elements that element.
        insertHTMLElementNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token->isEndTagToken() && containsTagName(InBodyFormattingTags, token->getTagName()))
    {
        if (findApproriatePlaceForInsertingNode()->tagName != token->getTagName())
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

void HTMLDocumentParser::processAfterBody(std::shared_ptr<HTMLToken> token)
{
    if (token->isCharacterToken() && token->isParserWhitespace())
    {
        //switchToState(InsertionMode::InBody);
        return;
    }

    if (token->isCommentToken())
    {
        insertCommentNode(token, findApproriatePlaceForInsertingNode());
        return;
    }

    if (token->isDoctypeToken())
    {
        ParseError();
    }

    if (token->isEndTagToken() && token->getTagName() == "html")
    {
        return;
    }
}

void HTMLDocumentParser::processText(std::shared_ptr<HTMLToken> token)
{
    if (token->isCharacterToken())
    {
        insertTextNode(token, findApproriatePlaceForInsertingNode());
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
        //printf("\nCurrent Token: \n	Type: %s\n Tag Name: %s\n", TokenTypesDocParser[token->getType()].c_str(), token->getTagName().c_str());

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

    printDOMTree();

}
