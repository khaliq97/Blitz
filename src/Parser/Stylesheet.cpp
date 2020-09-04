#include <Parser/Stylesheet.h>
std::string Stylesheet::stylesToString()
{
    std::string returnStr;
    for (auto style: styles)
    {
        returnStr += "Selector: " + style->selector + "\n";
        for (auto stylePropVal: *style->styleMap)
        {
            returnStr += "	{" + stylePropVal.first + ", " + stylePropVal.second + "}\n";
        }
    }

    return returnStr;
}
