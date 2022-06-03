#include <JS/VariableDeclaration.h>
#include <JS/BinaryExpression.h>
#include <JS/LiteralExpression.h>
#include <JS/UnaryExpression.h>
#include <JS/ExpressionStatement.h>
#include <fmt/core.h>
VariableDeclaration::VariableDeclaration(std::string name, std::shared_ptr<Expression> init, std::shared_ptr<VariableEnviroment> &variable_enviroment)
    : name(name), init(init), m_variable_enviroment(variable_enviroment) { }




std::shared_ptr<Value> VariableDeclaration::execute()
{
    auto declarative_enviroment_record = std::make_shared<DeclarativeEnviromentRecord>();

    //FIXME: This way of determining expression types is a bit ugly (yikes).
    if (auto maybeBinaryExpression = std::dynamic_pointer_cast<BinaryExpression>(init))
    {
        declarative_enviroment_record->m_record.insert(std::make_pair(name, maybeBinaryExpression->execute()));
    }

    if (auto maybeLiteralExpression = std::dynamic_pointer_cast<LiteralExpression>(init))
    {
        declarative_enviroment_record->m_record.insert(std::make_pair(name, maybeLiteralExpression->execute()));
    }

    if (auto maybeUnaryExpression = std::dynamic_pointer_cast<UnaryExpression>(init))
    {
        declarative_enviroment_record->m_record.insert(std::make_pair(name, maybeUnaryExpression->execute()));
    }

    if (auto maybeExpressionStatement = std::dynamic_pointer_cast<ExpressionStatement>(init))
    {
        declarative_enviroment_record->m_record.insert(std::make_pair(name, maybeExpressionStatement->execute()));
    }

    m_variable_enviroment->m_dec_env_records.push_back(declarative_enviroment_record);

    return std::make_shared<Value>(Value::ValueType::UNDEFINED);
}
