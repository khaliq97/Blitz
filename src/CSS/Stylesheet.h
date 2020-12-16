#ifndef STYLESHEET_H
#define STYLESHEET_H
#include <memory>
#include <vector>
#include <CSS/QualifiedRule.h>
#include <CSS/StyleRule.h>
class Stylesheet
{
public:
    Stylesheet();
    std::vector<std::shared_ptr<QualifiedRule>> rules;
    std::vector<std::shared_ptr<CSS::StyleRule>> styleRules;
private:



};

#endif // STYLESHEET_H
