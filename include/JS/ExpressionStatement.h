#ifndef EXPRESSIONSTATEMENT_H
#define EXPRESSIONSTATEMENT_H
#include <JS/ASTNode.h>
template <typename T>
class ExpressionStatement : public Expression<T>
{
    public:
        ExpressionStatement(std::shared_ptr<Expression<T>> expression)
            : expression(expression) { }
        std::shared_ptr<T> accept(ExpressionVisitor<T>& visitor)
        {
            return visitor.visitExpressionStatement(*this);
        }

        std::shared_ptr<Expression<T>> expression;

};

#endif // EXPRESSIONSTATEMENT_H
