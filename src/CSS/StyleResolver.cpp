#include <CSS/StyleResolver.h>
#include <Parser/Lexer.h>
static Lexer lexer;
StyleResolver::StyleResolver(const std::shared_ptr<Document>& document)
{
    this->documentWithCssDeclrations = document;

    std::vector<std::shared_ptr<Node>> empty = {};
    std::vector<std::shared_ptr<Node>> elements = documentWithCssDeclrations->getAllNodes(empty, documentWithCssDeclrations);

    for (auto elementNode: elements)
    {
        if (auto element = dynamic_cast<Element*>(elementNode.get()))
        {
            for (auto dec: element->declarations)
            {
                element->styleProperties.push_back(resolveStyleFromDeclaration(dec, elementNode));
            }
        }
    }
}
std::shared_ptr<StyleProperty> StyleResolver::resolveStyleFromDeclaration(const std::shared_ptr<Declaration> declaration, const std::shared_ptr<Node>& node)
{
   std::shared_ptr<StyleProperty> styleProp = std::make_shared<StyleProperty>(declaration);
   //Should be returning a list??!!
   for (auto declarationValue: declaration->value)
   {
       if (declarationValue->type == CSSTokenType::Dimension)
       {
           //font-size (em)
           if (lexer.caseInsensitiveStringCompare("em", declarationValue->unit))
           {
               // TODO: It needs to check if font-size already exists as a computed value
               // i.e. if the font-size already on the element is a dimension and has the "em" unit then we go back to the parent node.
               if (!findFirstDeclarationOccurence("font-size", node->parentNode))
               {
                    // Fallback font-size
                    styleProp->computedValue = 16 * declarationValue->valueAsDouble();
                    printf("Element: %s\n", node->nodeName.c_str());
                    printf("Fallback %s: %lf\n", declaration->name.c_str(), styleProp->computedValue);
                    return styleProp;
               }
               else
               {
                   styleProp->computedValue = findFirstDeclarationOccurence("font-size", node->parentNode)->computedValue * declarationValue->valueAsDouble();
                   printf("Element: %s\n", node->nodeName.c_str());
                   printf("Relative %s: %lf\n", declaration->name.c_str(), styleProp->computedValue);
                   return styleProp;
               }
           }
           else
           {
               styleProp->computedValue = declarationValue->valueAsDouble();
               return styleProp;
           }

       }
   }

   return styleProp;
}

std::shared_ptr<StyleProperty> StyleResolver::findFirstDeclarationOccurence(std::string declarationName, const std::shared_ptr<Node>& node)
{
    std::shared_ptr<Node> tempElementNode = node;
    std::shared_ptr<StyleProperty> styleProp;

    std::shared_ptr<Node> absoluteParent;
    while (tempElementNode)
    {
        if (auto element = dynamic_cast<Element*>(node.get()))
        {
            for (auto styleProp: element->styleProperties)
            {
                if (styleProp->m_declaration->name == declarationName)
                {
                   return styleProp;
                }
            }
        }

        absoluteParent = tempElementNode->parentNode;
        tempElementNode = absoluteParent;
    }

    return styleProp;
}

