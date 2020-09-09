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
#endif // LITERALEXPRESSION_H
