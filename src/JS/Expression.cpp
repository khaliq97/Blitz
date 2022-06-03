#include <JS/Expression.h>
#include <JS/LiteralExpression.h>
#include <JS/BinaryExpression.h>
#include<JS/ExpressionStatement.h>
#include <JS/UnaryExpression.h>
#include <fmt/core.h>
std::shared_ptr<Value> VariableExpression::execute()
{
    return value;
}

std::shared_ptr<Value> AssigmentExpression::execute()
{
   std::shared_ptr<Value> value = right->execute();
    for (auto record: m_variable_enviroment->m_dec_env_records)
    {
        // for (auto &[key, val] : record->m_record)
        // {
        //     fmt::print("Key: {} | Value: {}\n", key, val->toString());
        // }

        std::map<std::string, std::shared_ptr<Value>>::iterator it = record->m_record.find(left.lock()->lexeme);

         if (it != record->m_record.end())
         {
             it->second = value;
         }

    }

    return value;

}
