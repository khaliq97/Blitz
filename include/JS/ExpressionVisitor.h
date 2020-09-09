#ifndef EXPRESSIONVISITOR_H
#define EXPRESSIONVISITOR_H
#include <memory>
template<typename T> class Expression;
template<typename T> class BinaryExpression;
template<typename T> class LiteralExpression;
template<typename T> class UnaryExpression;
template<typename T> class ExpressionStatement;

template<class T>
class ExpressionVisitor
{
    public:
        virtual std::shared_ptr<T> visitBinaryExpression(BinaryExpression<T>& expr) = 0;
        virtual std::shared_ptr<T> visitLiteralExpression(LiteralExpression<T>& expr) = 0;
        virtual std::shared_ptr<T> visitUnaryExpression(UnaryExpression<T>& expr) = 0;
        virtual std::shared_ptr<T> visitExpressionStatement(ExpressionStatement<T>& expr) = 0;
};

#endif // EXPRESSIONVISITOR_H
