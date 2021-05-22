#include <stdio.h>
#include <vector>
#include <DOM/Document.h>
#include <fmt/core.h>
#include <LexerUtils.h>
Document::Document()
{

}

Document::Document(std::weak_ptr<Node> parentNode) : Node(parentNode)
{

}

void Document::printDocument(const std::shared_ptr<Node>& node, std::string indentation)
{
    fmt::print("{}<{}>\n", indentation.c_str(), node->nodeName.c_str());

    fmt::print("  {}Node Type: {}\n", indentation.c_str(), Tools::lookupNodeTypeFromDictionary(node->nodeType));
    fmt::print("  {}Node Name: {}\n", indentation.c_str(), node->nodeName);
    fmt::print("  {}Node Value: {}\n", indentation.c_str(), node->nodeValue);
    fmt::print("  {}Base URI: {}\n", indentation.c_str(), node->baseURI);
    fmt::print("  {}Is Connected: {}\n", indentation.c_str(), node->getIsConnected());
    fmt::print("  {}Text Content: {}\n", indentation.c_str(), node->getTextContent());
    fmt::print("  {}Parent: {}\n", indentation.c_str(), node->parentNode.lock() ? node->parentNode.lock()->nodeName : "<root>");
    fmt::print("  {}Previous Sibling: {}\n", indentation.c_str(), node->previousSibling ? node->previousSibling->nodeName : "<empty>");

    if (node->isComment())
    {
        std::weak_ptr<Comment> commentNode = node->asNodeTypeComment(node).lock();
        fmt::print("  {}Data: {}\n", indentation.c_str(), commentNode.lock()->data);
        fmt::print("  {}Length : {}\n", indentation.c_str(), commentNode.lock()->length);
    }


    if (node->isDocumentType())
    {
        std::weak_ptr<DocumentType> documentTypeNode = node->asNodeTypeDocumentType(node).lock();
        fmt::print("  {}Name: {}\n", indentation.c_str(), documentTypeNode.lock()->name());
        fmt::print("  {}Public ID : {}\n", indentation.c_str(), documentTypeNode.lock()->publicId());
        fmt::print("  {}System ID : {}\n", indentation.c_str(), documentTypeNode.lock()->systemId());
    }

    if (node->isElement())
    {
        std::weak_ptr<Element> elementNode = node->asNodeTypeElement(node).lock();

        fmt::print("  {}Namespace URI: {}\n", indentation.c_str(), elementNode.lock()->namespaceURI());
        fmt::print("  {}Prefix: {}\n", indentation.c_str(), elementNode.lock()->prefix());
        fmt::print("  {}Local Name: {}\n", indentation.c_str(), elementNode.lock()->localName());
        fmt::print("  {}Tag Name: {}\n", indentation.c_str(), elementNode.lock()->tagName());


        if (elementNode.lock()->hasAttributes())
        {
            fmt::print("  {}Attributes: \n", indentation.c_str());
            for (auto &attribute: elementNode.lock()->attributes)
            {
                fmt::print("  {}    {}={}\n", indentation.c_str(), attribute.first.c_str(), attribute.second.c_str());
            }
        }
    }

    if (node->isText())
    {
        std::weak_ptr<Text> textNode = node->asNodeTypeText(node).lock();
        fmt::print("  {}Data: {}\n", indentation.c_str(), textNode.lock()->data);
        fmt::print("  {}Length: {}\n", indentation.c_str(), textNode.lock()->length);
    }

    if (node->isElement())
    {
        fmt::print("{}\033[1;33m  Styles: \033[0m\n", indentation.c_str());
        for (auto dec: node->asNodeTypeElement(node).lock()->declarations)
        {
            fmt::print("    {}Declaration:\n", indentation.c_str());
            fmt::print("       {}Name: {}\n", indentation.c_str(), dec->name.c_str());
            for (auto declarationValue: dec->value)
            {
                fmt::print("       {}Value: {}\n", indentation.c_str(), declarationValue.value().c_str());
                if (declarationValue.type == CSSTokenType::Dimension)
                    fmt::print("           {}Unit: {}\n", indentation.c_str(), declarationValue.unit.c_str());
            }
        }

        fmt::print("{}\033[1;33m  Style Properties (computed): \033[0m\n", indentation.c_str());
        for (auto dec: node->asNodeTypeElement(node).lock()->styleProperties)
        {
            fmt::print("    {}Declaration:\n", indentation.c_str());
            fmt::print("       {}Name: {}\n", indentation.c_str(), dec->m_declaration->name);
                fmt::print("       {}Computed Value: {}\n", indentation.c_str(), dec->computedValue);
                if (dec->m_declaration->value[0].type == CSSTokenType::Dimension)
                    fmt::print("           {}Unit: px\n", indentation);
        }
    }

    indentation += "    ";

    for (int i = 0; i < node->childNodes.size(); i++)
    {
        printDocument(node->childNodes[i], indentation);
    }
}
