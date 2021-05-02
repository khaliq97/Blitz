#ifndef DECLARATION_H
#define DECLARATION_H
#include <memory>
#include <vector>
#include <CSS/CSSToken.h>
class Declaration
{
public:
    Declaration();
    std::string name;
    std::vector<CSSToken> value;
};

#endif // DECLARATION_H
