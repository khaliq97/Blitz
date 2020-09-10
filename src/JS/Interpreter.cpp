#include <JS/Interpreter.h>
Interpreter::Interpreter()
{

}

void Interpreter::intereprt(std::shared_ptr<Expression> expr)
{
    std::shared_ptr<Value> value = expr->execute();
    printf("%s\n", value->toString().c_str());
}

Interpreter::~Interpreter()
{

}

