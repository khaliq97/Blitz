#ifndef VARIABLEDECLARATION_H
#define VARIABLEDECLARATION_H
#include <JS/ASTNode.h>
#include <JS/Expression.h>
#include <JS/Enviroment/VariableEnviroment.h>
#include <JS/Enviroment/DeclarativeEnviromentRecord.h>
#include <vector>
#include <memory>

class VariableDeclaration : public Expression
{
    public:
        VariableDeclaration(std::string name, std::shared_ptr<Expression> init, std::shared_ptr<VariableEnviroment> &variable_enviroment);
        std::string name;
        std::shared_ptr<Expression> init;
        virtual std::shared_ptr<Value> execute() override;

    private:
        std::shared_ptr<VariableEnviroment> &m_variable_enviroment;

};

//class VariableDeclaration : public DeclarationStatement
//{
//    public:
//        std::vector<std::shared_ptr<VariableDeclarator>> declarations;
//        std::string kind;
//};


#endif // VARIABLEDECLARATION_H
