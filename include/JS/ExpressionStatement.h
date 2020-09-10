#ifndef EXPRESSIONSTATEMENT_H
#define EXPRESSIONSTATEMENT_H
#include <JS/ASTNode.h>
#include <JS/Expression.h>
class ExpressionStatement : public Expression
{
    public:
        ExpressionStatement(const std::shared_ptr<Expression>& expression)
            : expression(expression) { }

        virtual std::shared_ptr<Value> execute() override;

    private:
        std::shared_ptr<Expression> expression;

};

#endif // EXPRESSIONSTATEMENT_H
