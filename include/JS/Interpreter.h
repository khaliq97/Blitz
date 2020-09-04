#ifndef INTEPRETER_H
#define INTEPRETER_H
#include <vector>
#include <memory>
#include <ASTNode.h>
class Interpreter
{
    public:
        Interpreter();
    private:
        std::vector<std::shared_ptr<ASTNode>> astNodes;
};

#endif
