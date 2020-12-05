#ifndef VARIABLEDECLARATION_H
#define VARIABLEDECLARATION_H
#include <JS/ASTNode.h>
#include <JS/Expression.h>
#include <vector>
#include <memory>

class VariableDeclaration : public Expression
{
    public:
        VariableDeclaration(std::string id, const std::shared_ptr<Expression>& init) : id(id), init(init) { }
        std::string id;
        const std::shared_ptr<Expression>& init;
        virtual std::shared_ptr<Value> execute() override;
};

//class VariableDeclaration : public DeclarationStatement
//{
//    public:
//        std::vector<std::shared_ptr<VariableDeclarator>> declarations;
//        std::string kind;
//};


#endif // VARIABLEDECLARATION_H
