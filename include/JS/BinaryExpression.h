#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H
#include <JS/ASTNode.h>
#include <JS/Token.h>
#include <string>
#include <memory>
template<typename T>
class BinaryExpression : public Expression<T>
{
    public:
        BinaryExpression(std::shared_ptr<Expression<T>> left, std::shared_ptr<Token> op, std::shared_ptr<Expression<T>> right)
            : left(std::move(left)), op(op), right(std::move(right)){ }

        std::shared_ptr<T> accept(ExpressionVisitor<T>& visitor)
        {
            return visitor.visitBinaryExpression(*this);
        }

        std::shared_ptr<Expression<T>> left;
        std::shared_ptr<Token> op;
        std::shared_ptr<Expression<T>> right;
};

#endif // BINARYEXPRESSION_H
