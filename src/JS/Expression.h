#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <JS/ASTNode.h>
#include <JS/Token.h>
class Expression : public ASTNode
{
    public:

};

class VariableExpression : public Expression
{
public:
    VariableExpression(std::shared_ptr<Value> value) : value(value) { }
    virtual std::shared_ptr<Value> execute() override;

private:
    std::shared_ptr<Value> value;
};

class AssigmentExpression : public Expression
{
public:
    AssigmentExpression(std::weak_ptr<Token> left, const std::shared_ptr<Token>& op, const std::shared_ptr<Expression>& right,
                        std::shared_ptr<VariableEnviroment> &variable_enviroment)
        : left(left), op(op), right(right), m_variable_enviroment(variable_enviroment){ }

    virtual std::shared_ptr<Value> execute() override;
    std::shared_ptr<VariableEnviroment> m_variable_enviroment;

private:
    std::weak_ptr<Token> left;
    std::shared_ptr<Token> op;
    std::shared_ptr<Expression> right;

};

#endif // EXPRESSION_H
