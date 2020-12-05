#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H
#include <JS/ASTNode.h>
#include <JS/Token.h>
#include <JS/Expression.h>
#include <string>
#include <memory>
class BinaryExpression : public Expression
{
    public:
        BinaryExpression(const std::shared_ptr<Expression>& left, const std::shared_ptr<Token>& op, const std::shared_ptr<Expression>& right)
            : left(left), op(op), right(right){ }

        virtual std::shared_ptr<Value> execute() override;

    private:
        std::shared_ptr<Expression> left;
        std::shared_ptr<Token> op;
        std::shared_ptr<Expression> right;
};

#endif // BINARYEXPRESSION_H
