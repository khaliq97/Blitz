#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <memory>
#include <JS/Token.h>
#include <JS/Expression.h>
#include <JS/LiteralExpression.h>
#include <JS/ExpressionStatement.h>
#include <JS/BinaryExpression.h>
#include <JS/UnaryExpression.h>
#include <JS/Value.h>
#include <JS/Error.h>
class Parser
{
    public:
        Parser(std::vector<std::shared_ptr<Token>> tokens) : tokens(tokens) { }
        std::shared_ptr<Expression<Value>> parse();
        std::shared_ptr<Expression<Value>> expression();
        std::shared_ptr<Expression<Value>> equality();
        std::shared_ptr<Expression<Value>> additon();
        std::shared_ptr<Expression<Value>> multiplication();
        std::shared_ptr<Expression<Value>> comparison();
        std::shared_ptr<Expression<Value>> unary();
        std::shared_ptr<Expression<Value>> primary();

        std::shared_ptr<Token> consume(TokenType type, std::string message);
        std::unique_ptr<SyntaxError> error(std::shared_ptr<Token> token, std::string message);
        void synchronize();
        bool check(TokenType type);
        bool match(std::vector<TokenType> types);
        std::shared_ptr<Token> advance();
        bool isAtEnd();
        std::shared_ptr<Token> peek();
        std::shared_ptr<Token> previous();

        int currentParserToken = 0;


    private:
        std::vector<std::shared_ptr<Token>> tokens;
};

#endif // PARSER_H
