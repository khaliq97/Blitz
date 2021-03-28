#include <CSS/StyleResolver.h>
#include <Parser/Lexer.h>

static Lexer lexer;

StyleResolver::StyleResolver(const std::shared_ptr<Document> &document)
{
    this->documentWithCSSDeclrations = document;

    std::vector<std::shared_ptr<Node>> empty = {};
    std::vector<std::shared_ptr<Node>> elements = documentWithCSSDeclrations->getAllNodes(empty, documentWithCSSDeclrations);

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
               if (!findFirstDeclarationOccurence("font-size", node->parentNode))
               {
                    // Fallback font-size
                    styleProp->computedValue = 16 * declarationValue->valueAsDouble();
                    return styleProp;
               }
               else
               {
                   // Declaration defined font-size
                   styleProp->computedValue = findFirstDeclarationOccurence("font-size", node->parentNode)->computedValue * declarationValue->valueAsDouble();
                   return styleProp;
               }
           }
           else
           {
               styleProp->computedValue = declarationValue->valueAsDouble();
               return styleProp;
           }

       }
       else if (isColor(declarationValue->value()))
       {

           styleProp->color = convertColorToRGB(declarationValue->value());
           return styleProp;
       }
   }

   return styleProp;
}

std::shared_ptr<Color> StyleResolver::convertColorToRGB(std::string color)
{
    if (lexer.caseInsensitiveStringCompare("black", color))
    {
        return std::make_shared<Color>(0, 0, 0);
    }
    else if (lexer.caseInsensitiveStringCompare("silver", color))
    {
        return std::make_shared<Color>(192, 192, 192);
    }
    else if (lexer.caseInsensitiveStringCompare("gray", color))
    {
        return std::make_shared<Color>(128,128,128);
    }else if (lexer.caseInsensitiveStringCompare("white", color))
    {
        return std::make_shared<Color>(255,255,255);
    }else if (lexer.caseInsensitiveStringCompare("maroon", color))
    {
        return std::make_shared<Color>(128,0,0);
    }else if (lexer.caseInsensitiveStringCompare("red", color))
    {
        return std::make_shared<Color>(255,0,0 );
    }else if (lexer.caseInsensitiveStringCompare("purple", color))
    {
        return std::make_shared<Color>(128,0,128);
    }else if (lexer.caseInsensitiveStringCompare("fuchsia", color))
    {
        return std::make_shared<Color>(255,0,255);
    }else if (lexer.caseInsensitiveStringCompare("green", color))
    {
        return std::make_shared<Color>(0,128,0 );
    }else if (lexer.caseInsensitiveStringCompare("olive", color))
    {
        return std::make_shared<Color>(0,255,0 );
    }else if (lexer.caseInsensitiveStringCompare("yellow", color))
    {
        return std::make_shared<Color>(255, 255, 0);
    }else if (lexer.caseInsensitiveStringCompare("navy", color))
    {
        return std::make_shared<Color>(0, 0, 128);
    }else if (lexer.caseInsensitiveStringCompare("blue", color))
    {
        return std::make_shared<Color>(0, 0, 255);
    }else if (lexer.caseInsensitiveStringCompare("teal", color))
    {
        return std::make_shared<Color>(0, 128, 128);
    }else if (lexer.caseInsensitiveStringCompare("aqua", color))
    {
        return std::make_shared<Color>(0, 255, 255);
    }
    else {
        return std::make_shared<Color>(0, 0, 0);
    }
}

bool StyleResolver::isColor(std::string value)
{
    if (value == "black" ||
        value == "silver" ||
        value == "gray" ||
        value == "white" ||
        value == "maroon" ||
        value == "red" ||
        value == "purple" ||
        value == "fuchsia" ||
        value == "green" ||
        value == "lime" ||
        value == "olive" ||
        value == "yellow" ||
        value == "navy" ||
        value == "blue" ||
        value == "teal" ||
        value == "aqua")
    {
        return true;
    }

    return false;
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

