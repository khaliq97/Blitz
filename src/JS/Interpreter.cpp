#include <JS/Interpreter.h>
Interpreter::Interpreter()
{

}

void Interpreter::intereprt(std::shared_ptr<Expression> expr)
{
    std::shared_ptr<Value> value = expr->execute();
    if (value->type == Value::ValueType::NUMBER || value->type == Value::ValueType::BOOL )
    {
        printf("\033[33m%s\033[0m\n", value->toString().c_str());
    }

    if (value->type == Value::ValueType::STRING)
    {
        printf("\033[32m'%s'\033[0m\n", value->toString().c_str());
    }

    if (value->type == Value::ValueType::UNDEFINED)
    {
        printf("\033[30m%s\033[0m\n", value->toString().c_str());
    }


    //printf("%s\n", value->toString().c_str());
}

Interpreter::~Interpreter()
{

}

