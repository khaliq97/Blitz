#ifndef INTEPRETER_H
#define INTEPRETER_H
#include <vector>
#include <memory>
#include <JS/ASTNode.h>
#include <JS/Value.h>
#include <JS/ExpressionVisitor.h>
#include <JS/BinaryExpression.h>
#include <JS/LiteralExpression.h>
#include <JS/ExpressionStatement.h>
#include <JS/UnaryExpression.h>
#include <JS/Expression.h>
#include <JS/TokenType.h>
class Interpreter : public ExpressionVisitor<Value>
{
    public:
        virtual ~Interpreter();
        Interpreter();

        void intereprt(std::shared_ptr<Expression<Value>> expr);

        std::shared_ptr<Value> evaluate(std::shared_ptr<Expression<Value>> expr)
        {
            return expr->accept(*this);
        }

        virtual std::shared_ptr<Value> visitExpression(Expression<Value>& expr) override
        {

        }

        virtual std::shared_ptr<Value> visitLiteralExpression(LiteralExpression<Value>& expr) override
        {
            return expr.m_value;
        }

        virtual std::shared_ptr<Value> visitExpressionStatement(ExpressionStatement<Value>& expr) override
        {
            return evaluate(expr.expression);
        }

        virtual std::shared_ptr<Value> visitUnaryExpression(UnaryExpression<Value>& expr) override
        {
            std::shared_ptr<Value> right = evaluate(expr.right);

            switch (expr.op->type)
            {
                case TokenType::MINUS:
                   return std::make_shared<Value>(-right->m_value.asDouble);
            }
        }

        virtual std::shared_ptr<Value> visitBinaryExpression(BinaryExpression<Value>& expr) override
        {
            std::shared_ptr<Value> left = evaluate(expr.left);
            std::shared_ptr<Value> right = evaluate(expr.right);

            switch (expr.op->type)
            {
                case TokenType::MINUS:
                    return std::make_shared<Value>(left->m_value.asDouble - right->m_value.asDouble);
                case TokenType::SLASH:
                    return std::make_shared<Value>(left->m_value.asDouble / right->m_value.asDouble);
                case TokenType::STAR:
                    return std::make_shared<Value>(left->m_value.asDouble * right->m_value.asDouble);
                case TokenType::PLUS:
                    if (left->type == Value::ValueType::NUMBER && right->type == Value::ValueType::NUMBER)
                    {
                        return std::make_shared<Value>(left->m_value.asDouble + right->m_value.asDouble);
                    }
                    else if (left->type == Value::ValueType::STRING && right->type == Value::ValueType::STRING)
                    {
                        return std::make_shared<Value>(left->m_value.asString.append(right->m_value.asString));
                    }
            }
        }


    private:

};

#endif
