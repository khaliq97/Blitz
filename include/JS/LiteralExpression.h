#ifndef LITERALEXPRESSION_H
#define LITERALEXPRESSION_H
#include <JS/Expression.h>
#include <JS/Value.h>
#include <string>
class LiteralExpression : public Expression
{
    public:
        virtual ~LiteralExpression();

};

class NumericLiteral : public LiteralExpression
{
    public:
        NumericLiteral(double value)
            : value(value)
        {

        }
        virtual std::shared_ptr<Value> execute() override;
    private:
        double value;
};

class StringLiteral : public LiteralExpression
{
    public:
        StringLiteral(std::string value)
            : value(value)
        {

        }
        virtual std::shared_ptr<Value> execute() override;
    private:
        std::string value;
};

class BooleanLiteral : public LiteralExpression
{
    public:
        BooleanLiteral(bool value)
            : value(value)
        {

        }
        virtual std::shared_ptr<Value> execute() override;
    private:
        bool value;
};

#endif // LITERALEXPRESSION_H
