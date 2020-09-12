#include <JS/BinaryExpression.h>

std::shared_ptr<Value> BinaryExpression::execute()
{
    std::shared_ptr<Value> leftExpr = left->execute();
    std::shared_ptr<Value> rightExpr = right->execute();

    switch (op->type)
    {
        case TokenType::MINUS:
            return std::make_shared<Value>(leftExpr->m_value.asDouble - rightExpr->m_value.asDouble);
        case TokenType::SLASH:
            return std::make_shared<Value>(leftExpr->m_value.asDouble / rightExpr->m_value.asDouble);
        case TokenType::STAR:
            return std::make_shared<Value>(leftExpr->m_value.asDouble * rightExpr->m_value.asDouble);
        case TokenType::PLUS:
            if (leftExpr->type == Value::ValueType::NUMBER && rightExpr->type == Value::ValueType::NUMBER)
            {
                return std::make_shared<Value>(leftExpr->m_value.asDouble + rightExpr->m_value.asDouble);
            }
            else if (leftExpr->type == Value::ValueType::STRING && rightExpr->type == Value::ValueType::STRING)
            {
                return std::make_shared<Value>(leftExpr->m_value.asString.append(rightExpr->m_value.asString));
            }
            break;
        case TokenType::EQUAL_EQUAL:
            if (leftExpr->type == Value::ValueType::NUMBER && rightExpr->type == Value::ValueType::NUMBER)
            {
                // FIXME: Unsafe comparison of floating point types
                return std::make_shared<Value>(leftExpr->m_value.asDouble == rightExpr->m_value.asDouble);
            }
            break;
        case TokenType::GREATER:
            if (leftExpr->type == Value::ValueType::NUMBER && rightExpr->type == Value::ValueType::NUMBER)
            {
                return std::make_shared<Value>(leftExpr->m_value.asDouble > rightExpr->m_value.asDouble);
            }
            break;
        case TokenType::GREATER_EQUAL:
            if (leftExpr->type == Value::ValueType::NUMBER && rightExpr->type == Value::ValueType::NUMBER)
            {
                return std::make_shared<Value>(leftExpr->m_value.asDouble >= rightExpr->m_value.asDouble);
            }
            break;
        case TokenType::LESS:
            if (leftExpr->type == Value::ValueType::NUMBER && rightExpr->type == Value::ValueType::NUMBER)
            {
                return std::make_shared<Value>(leftExpr->m_value.asDouble < rightExpr->m_value.asDouble);
            }
            break;
        case TokenType::LESS_EQUAL:
            if (leftExpr->type == Value::ValueType::NUMBER && rightExpr->type == Value::ValueType::NUMBER)
            {
                return std::make_shared<Value>(leftExpr->m_value.asDouble <= rightExpr->m_value.asDouble);
            }
            break;
    }
}
