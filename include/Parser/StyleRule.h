#ifndef STYLERULE_H
#define STYLERULE_H
#include <string>
#include <map>
class StyleRule
{
    public:
        StyleRule();
        StyleRule(std::string selector);
        std::string selector;
        std::map<std::string, std::string> *styleMap;
};

#endif // STYLERULE_H
