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
                if (lexer.caseInsensitiveStringCompare(dec->name, "padding"))
                {
                    for (auto styleProp: resolvePosistionalPropertyValues(ShortHandPropertyNameType::Padding, dec))
                    {
                         element->styleProperties.push_back(resolveStyleFromDeclaration(styleProp->m_declaration, elementNode));
                    }
                }
                else if (lexer.caseInsensitiveStringCompare(dec->name, "margin"))
                {
                    for (auto styleProp: resolvePosistionalPropertyValues(ShortHandPropertyNameType::Margin, dec))
                    {
                         element->styleProperties.push_back(resolveStyleFromDeclaration(styleProp->m_declaration, elementNode));
                    }
                }
                else if (lexer.caseInsensitiveStringCompare(dec->name, "border"))
                {
                    for (auto styleProp: resolvePosistionalPropertyValues(ShortHandPropertyNameType::Border, dec))
                    {
                         element->styleProperties.push_back(resolveStyleFromDeclaration(styleProp->m_declaration, elementNode));
                    }
                }
                else
                {
                    element->styleProperties.push_back(resolveStyleFromDeclaration(dec, elementNode));
                }
            }
        }
    }
}

std::shared_ptr<StyleProperty> StyleResolver::createPositionalStyleProperty(std::string declarationName, const std::shared_ptr<CSSToken>& declarationValue)
{
    std::shared_ptr<Declaration> declaration = std::make_shared<Declaration>();
    declaration->name = declarationName;
    declaration->value.push_back(declarationValue);

    return std::make_shared<StyleProperty>(declaration);
}

std::vector<std::shared_ptr<StyleProperty>> StyleResolver::resolvePosistionalPropertyValues(ShortHandPropertyNameType type, const std::shared_ptr<Declaration> declaration)
{
    std::vector<std::shared_ptr<StyleProperty>> resovledStyleProperties;
    int declarationValueCount = declaration->value.size();


    switch (type)
    {
        case ShortHandPropertyNameType::Margin:
            if (declarationValueCount == 1)
            {
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-top", declaration->value[0])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-bottom", declaration->value[0])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-left", declaration->value[0])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-right", declaration->value[0])));
            }
            else if (declarationValueCount == 2)
            {
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-top", declaration->value[0])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-bottom", declaration->value[0])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-left", declaration->value[1])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-right", declaration->value[1])));
            }
            else if (declarationValueCount == 3)
            {
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-top", declaration->value[0])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-bottom", declaration->value[2])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-left", declaration->value[1])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-right", declaration->value[1])));
            }
            else if (declarationValueCount == 4)
            {
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-top", declaration->value[0])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-bottom", declaration->value[2])));
                resovledStyleProperties.push_back(std::move(createPositionalStyleProperty("margin-left", declaration->value[3])));
                resovledStyleProperties.push_back(createPositionalStyleProperty("margin-right", declaration->value[1]));
            }
            break;

        case ShortHandPropertyNameType::Padding:
            if (declarationValueCount == 1)
            {
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-top", declaration->value[0]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-bottom", declaration->value[0]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-left", declaration->value[0]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-right", declaration->value[0]));
            }
            else if (declarationValueCount == 2)
            {
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-top", declaration->value[0]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-bottom", declaration->value[0]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-left", declaration->value[1]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-right", declaration->value[1]));
            }
            else if (declarationValueCount == 3)
            {
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-top", declaration->value[0]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-bottom", declaration->value[2]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-left", declaration->value[1]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-right", declaration->value[1]));
            }
            else if (declarationValueCount == 4)
            {
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-top", declaration->value[0]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-bottom", declaration->value[2]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("padding-left", declaration->value[3]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("margin-padding", declaration->value[1]));
            }
            break;
        case ShortHandPropertyNameType::Border:
            if (declarationValueCount == 3)
            {
                resovledStyleProperties.push_back(createPositionalStyleProperty("border-top-width", declaration->value[0]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("border-bottom-width", declaration->value[0]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("border-left-width", declaration->value[0]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("border-right-width", declaration->value[0]));

                resovledStyleProperties.push_back(createPositionalStyleProperty("border-top-style", declaration->value[1]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("border-bottom-style", declaration->value[1]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("border-left-style", declaration->value[1]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("border-right-style", declaration->value[1]));

                resovledStyleProperties.push_back(createPositionalStyleProperty("border-top-color", declaration->value[2]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("border-bottom-color", declaration->value[2]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("border-left-color", declaration->value[2]));
                resovledStyleProperties.push_back(createPositionalStyleProperty("border-right-color", declaration->value[2]));
            }


    }

    return resovledStyleProperties;
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
                    //printf("Element: %s\n", node->nodeName.c_str());
                    //printf("Fallback %s: %lf\n", declaration->name.c_str(), styleProp->computedValue);
                    return styleProp;
               }
               else
               {
                   styleProp->computedValue = findFirstDeclarationOccurence("font-size", node->parentNode)->computedValue * declarationValue->valueAsDouble();
                   //printf("Element: %s\n", node->nodeName.c_str());
                   //printf("Relative %s: %lf\n", declaration->name.c_str(), styleProp->computedValue);
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

