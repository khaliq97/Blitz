#ifndef UNARYEXPRESSION_H
#define UNARYEXPRESSION_H
#include <JS/ASTNode.h>
#include <JS/Token.h>
#include <JS/Expression.h>
#include <string>
#include <memory>
class UnaryExpression : public Expression
{
    public:
        UnaryExpression(const std::shared_ptr<Token>& op, const std::shared_ptr<Expression>& right)
            : op(op), right(right) { }

        virtual std::shared_ptr<Value> execute() override;

    private:
        std::shared_ptr<Token> op;
        std::shared_ptr<Expression> right;
};
#endif // UNARYEXPRESSION_H
