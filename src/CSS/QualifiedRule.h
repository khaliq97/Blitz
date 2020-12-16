#ifndef QUALIFIEDRULE_H
#define QUALIFIEDRULE_H
#include <memory>
#include <CSS/SimpleBlock.h>
#include <vector>
#include <CSS/CSSToken.h>
class QualifiedRule
{
public:
    QualifiedRule();
    std::vector<std::shared_ptr<CSSToken>> prelude;
    std::shared_ptr<SimpleBlock> simpleBlock;
};

#endif // QUALIFIEDRULE_H
