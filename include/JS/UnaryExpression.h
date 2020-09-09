#ifndef UNARYEXPRESSION_H
#define UNARYEXPRESSION_H
#include <JS/ASTNode.h>
#include <JS/Token.h>
#include <string>
#include <memory>
template<typename T>
class UnaryExpression : public Expression<T>
{
    public:
        UnaryExpression(std::shared_ptr<Token> op, std::shared_ptr<Expression<T>> right)
            : op(op), right(std::move(right)){ }

        std::shared_ptr<T> accept(ExpressionVisitor<T>& visitor)
        {
            return visitor.visitUnaryExpression(*this);
        }

        std::shared_ptr<Token> op;
        std::shared_ptr<Expression<T>> right;
};
#endif // UNARYEXPRESSION_H
