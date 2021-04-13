#include <stdio.h>
#include <vector>
#include <DOM/Document.h>
#include <Parser/Lexer.h>
Document::Document()
{

}

Document::Document(std::shared_ptr<Node> parentNode) : Node(parentNode)
{

}

void Document::printTree(const std::shared_ptr<Node>& node, std::string indent, bool last)
{
    printf("%s%s\033[1;32m%d\033[0m\n", indent.c_str(), "\033[1;32m+- \033[0m", node->nodeType);
    printf("%s%s%s\n", indent.c_str(), " Node Name: ", node->nodeName.c_str());
    printf("%s%s%s\n", indent.c_str(), " Node Value: ", node->nodeValue.c_str());
    printf("%s%s%s\n", indent.c_str(), " Base URI: ", node->baseURI.c_str());
    printf("%s%s%s\n", indent.c_str(), " Is Connected: ", node->getIsConnected().c_str());
    printf("%s%s%s\n", indent.c_str(), " Text Content: ", node->getTextContent().c_str());

    if (node->nextSibling)
        printf("%s%s%s\n", indent.c_str(), " Next Sibling Node Name: ", node->nextSibling->nodeName.c_str());

    if (node->previousSibling)
        printf("%s%s%s\n", indent.c_str(), " Previous Sibling Node Name: ", node->previousSibling->nodeName.c_str());

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

         printf("    Styles: \n");
        for (auto dec: elementNode->declarations)
        {
            printf("        Declaration\n");
            printf("            Name: %s\n", dec->name.c_str());
            for (auto declarationValue: dec->value)
            {
                printf("            Value: %s\n", declarationValue->value().c_str());
                if (declarationValue->type == CSSTokenType::Dimension)
                    printf("                Unit: %s\n", declarationValue->unit.c_str());
            }
        }
    }

    indent += last ? " " : "|  ";

    for (int i = 0; i < node->childNodes.size(); i++)
    {

        printTree(node->childNodes[i], indent, i == node->childNodes.size() - 1);
    }
}
