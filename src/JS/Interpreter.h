#ifndef INTEPRETER_H
#define INTEPRETER_H
#include <vector>
#include <memory>
#include <JS/ASTNode.h>
#include <JS/Value.h>
#include <JS/Expression.h>
#include <JS/TokenType.h>
class Interpreter
{
    public:
        virtual ~Interpreter();
        Interpreter();

        void intereprt(std::shared_ptr<Expression> expr);

    private:

};

#endif
