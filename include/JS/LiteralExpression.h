#ifndef LITERALEXPRESSION_H
#define LITERALEXPRESSION_H
#include <JS/Expression.h>
#include <JS/Value.h>
#include <string>
template<typename T>
class LiteralExpression : public Expression<T>
{
    public:
        LiteralExpression(bool value)
        {
            m_value = std::make_shared<Value>(value);
        }

        LiteralExpression(double value)
        {
            m_value = std::make_shared<Value>(value);
        }

        LiteralExpression(std::string value)
        {
            m_value = std::make_shared<Value>(value);
        }

        std::shared_ptr<T> accept(ExpressionVisitor<T>& visitor)
        {
            return visitor.visitLiteralExpression(*this);
        }

        std::shared_ptr<Value> m_value;
};

template<typename T>
class NumericLiteral : public LiteralExpression<T>
{
    public:
        NumericLiteral(double value)
            : value(value)
        {

        }
        double value;
};

template<typename T>
class StringLiteral : public LiteralExpression<T>
{
    public:
        StringLiteral(std::string value)
            : value(value)
        {

        }
        std::string value;
};

template<typename T>
class BoolLiteral : public LiteralExpression<T>
{
    public:
        BoolLiteral(bool value)
            : value(value)
        {

        }
        bool value;
};

#endif // LITERALEXPRESSION_H
