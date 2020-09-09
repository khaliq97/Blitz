#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <JS/ASTNode.h>
template <typename T>
class Expression : public ASTNode<T>
{
    public:
        std::shared_ptr<T> accept(ExpressionVisitor<T>& visitor)
        {
            return visitor.visitExpression(*this);
        }

};

#endif // EXPRESSION_H
