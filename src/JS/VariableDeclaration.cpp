#include <JS/VariableDeclaration.h>
std::shared_ptr<Value> VariableDeclaration::execute()
{
    return std::make_shared<Value>(Value::ValueType::UNDEFINED);
}
