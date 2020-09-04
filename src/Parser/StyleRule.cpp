#include <Parser/StyleRule.h>
StyleRule::StyleRule(std::string selector)
{
    this->selector = selector;
    this->styleMap = new std::map<std::string, std::string>();
}

