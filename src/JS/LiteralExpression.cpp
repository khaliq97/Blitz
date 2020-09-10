#include <JS/LiteralExpression.h>
LiteralExpression::~LiteralExpression()
{

}

std::shared_ptr<Value> NumericLiteral::execute()
{
    return std::make_shared<Value>(value);
}

std::shared_ptr<Value> StringLiteral::execute()
{
    return std::make_shared<Value>(value);
}

std::shared_ptr<Value> BooleanLiteral::execute()
{
    return std::make_shared<Value>(value);
}

