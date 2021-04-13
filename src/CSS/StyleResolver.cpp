#include <CSS/StyleResolver.h>

StyleResolver::StyleResolver(std::shared_ptr<Document> document)
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
           if (Tools::caseInsensitiveStringCompare("em", declarationValue->unit))
           {
               if (findFirstDeclarationOccurence("font-size", node))
               {
                   styleProp->computedValue = findFirstDeclarationOccurence("font-size", node)->computedValue * declarationValue->valueAsDouble();
                   return styleProp;
               }

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
    if (Tools::caseInsensitiveStringCompare("black", color))
    {
        return std::make_shared<Color>(0, 0, 0);
    }
    else if (Tools::caseInsensitiveStringCompare("silver", color))
    {
        return std::make_shared<Color>(192, 192, 192);
    }
    else if (Tools::caseInsensitiveStringCompare("gray", color))
    {
        return std::make_shared<Color>(128,128,128);
    }else if (Tools::caseInsensitiveStringCompare("white", color))
    {
        return std::make_shared<Color>(255,255,255);
    }else if (Tools::caseInsensitiveStringCompare("maroon", color))
    {
        return std::make_shared<Color>(128,0,0);
    }else if (Tools::caseInsensitiveStringCompare("red", color))
    {
        return std::make_shared<Color>(255,0,0 );
    }else if (Tools::caseInsensitiveStringCompare("purple", color))
    {
        return std::make_shared<Color>(128,0,128);
    }else if (Tools::caseInsensitiveStringCompare("fuchsia", color))
    {
        return std::make_shared<Color>(255,0,255);
    }else if (Tools::caseInsensitiveStringCompare("green", color))
    {
        return std::make_shared<Color>(0,128,0 );
    }else if (Tools::caseInsensitiveStringCompare("olive", color))
    {
        return std::make_shared<Color>(0,255,0 );
    }else if (Tools::caseInsensitiveStringCompare("yellow", color))
    {
        return std::make_shared<Color>(255, 255, 0);
    }else if (Tools::caseInsensitiveStringCompare("navy", color))
    {
        return std::make_shared<Color>(0, 0, 128);
    }else if (Tools::caseInsensitiveStringCompare("blue", color))
    {
        return std::make_shared<Color>(0, 0, 255);
    }else if (Tools::caseInsensitiveStringCompare("teal", color))
    {
        return std::make_shared<Color>(0, 128, 128);
    }else if (Tools::caseInsensitiveStringCompare("aqua", color))
    {
        return std::make_shared<Color>(0, 255, 255);
    }else if (Tools::caseInsensitiveStringCompare("orange", color))
    {
        return std::make_shared<Color>(255, 165, 0);
    }
    else {
        printf("\033[1;33mbold [WARN] CSS: Color not supported (%s) \033[0m\n", color.c_str());
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
        value == "aqua" ||
        value == "orange")
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

