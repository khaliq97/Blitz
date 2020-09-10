#include <JS/UnaryExpression.h>

std::shared_ptr<Value> UnaryExpression::execute()
{
    std::shared_ptr<Value> rightExpr = right->execute();

    switch (op->type)
    {
        case TokenType::MINUS:
           return std::make_shared<Value>(-rightExpr->m_value.asDouble);
    }
}
