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
    std::vector<std::shared_ptr<CSSToken>> value;
};

#endif // DECLARATION_H
