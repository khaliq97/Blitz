#include <JS/Interpreter.h>
Interpreter::Interpreter()
{

}

void Interpreter::intereprt(std::shared_ptr<Expression<Value> > expr)
{
    std::shared_ptr<Value> value = evaluate(expr);
    printf("%s\n", value->toString().c_str());
}

Interpreter::~Interpreter()
{

}

