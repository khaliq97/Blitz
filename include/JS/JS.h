#ifndef JS_H
#define JS_H
#include <string>
#include <JS/ASTNode.h>
#include <JS/Expression.h>
#include <JS/BinaryExpression.h>
class JS
{
    public:
        JS() { }
        JS(std::string fileName);
        void runPrompt();
        void run(std::string source);
        void error(int line, std::string message);
        void error(std::shared_ptr<Token> token, std::string message);
        void report(int line, std::string where, std::string message);
        void runFile(std::string path);

};

#endif // JS_H
