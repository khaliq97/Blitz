#ifndef ASTNODE_H
#define ASTNODE_H
#include <JS/ExpressionVisitor.h>
template<typename T>
class ASTNode
{
    public:
        virtual std::shared_ptr<T> accept(ExpressionVisitor<T>& visitor) = 0;

};

#endif // ASTNODE_H
