#include <JS/ExpressionStatement.h>
std::shared_ptr<Value> ExpressionStatement::execute()
{
    return expression->execute();
}
