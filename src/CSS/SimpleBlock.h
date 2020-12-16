#ifndef SIMPLEBLOCK_H
#define SIMPLEBLOCK_H
#include <vector>
#include <memory>
#include <CSS/CSSToken.h>
class SimpleBlock
{
public:

    std::vector<std::shared_ptr<CSSToken>> values;
};

#endif // SIMPLEBLOCK_H
