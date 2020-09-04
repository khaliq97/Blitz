#ifndef STYLESHEET_H
#define STYLESHEET_H
#include <vector>
#include <memory>
#include <Parser/StyleRule.h>
class Stylesheet
{
    public:
        std::vector<std::shared_ptr<StyleRule>> styles;
        std::string stylesToString();
};

#endif // STYLESHEET_H
