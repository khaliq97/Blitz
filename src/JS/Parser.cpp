﻿#include "Parser.h"

#include <JS/Parser.h>
#include <JS/JS.h>
#include <fmt/core.h>
static JS js;

Parser::Parser(std::vector<std::shared_ptr<Token> > tokens, std::shared_ptr<VariableEnviroment> &m_variable_enviroment) : tokens(tokens), m_variable_enviroment(m_variable_enviroment)
{

}

std::shared_ptr<Expression> Parser::parse()
{
    try {
        return declaration();
    } catch (SyntaxError error) {
        printf("%s\n%s", error.name.c_str(), error.message.c_str());
        return nullptr;
    }
}

std::shared_ptr<Expression> Parser::expression()
{
    return assigment_expression();
}

std::shared_ptr<Expression> Parser::assigment_expression()
{
    auto expression = equality();
    std::weak_ptr<Token> indetifierToken;

    if (match({TokenType::EQUAL}))
    {
        int index = currentParserToken - 1;

        while (index >=0)
        {
            if (tokens[index]->type == TokenType::IDENTIFIER)
            {
                indetifierToken = tokens[index];
                break;
            }
            index--;
        }

        auto equals = previous();
        auto value = assigment_expression();

        return std::make_shared<AssigmentExpression>(indetifierToken, equals, value, m_variable_enviroment);
    }

    return expression;
}

std::shared_ptr<Expression> Parser::statement()
{
    return expression();
}

std::shared_ptr<Expression> Parser::declaration()
{
    if (match({TokenType::VAR}))
    {
        return varDeclaration();
    }

    return statement();
}

std::shared_ptr<Expression> Parser::equality()
{
    std::shared_ptr<Expression> expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
         std::shared_ptr<Token> op = previous();
         std::shared_ptr<Expression> right = comparison();
         expr = std::make_shared<BinaryExpression>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expression> Parser::comparison()
{
     std::shared_ptr<Expression> expr = additon();
     while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
     {
          std::shared_ptr<Token> op = previous();
          std::shared_ptr<Expression> right = additon();
          expr = std::make_shared<BinaryExpression>(expr, op, right);
     }

     return expr;
}

std::shared_ptr<Expression> Parser::additon()
{
     std::shared_ptr<Expression> expr = multiplication();
     while (match({TokenType::MINUS, TokenType::PLUS}))
     {
          std::shared_ptr<Token> op = previous();
          std::shared_ptr<Expression> right = multiplication();
          expr = std::make_shared<BinaryExpression>(expr, op, right);
     }

     return expr;
}

std::shared_ptr<Expression> Parser::multiplication()
{
    std::shared_ptr<Expression> expr = unary();
    while (match({TokenType::SLASH, TokenType::STAR}))
    {
         std::shared_ptr<Token> op = previous();
         std::shared_ptr<Expression> right = unary();
         expr = std::make_shared<BinaryExpression>(expr, op, right);
    }

    return expr;
}


std::shared_ptr<Expression> Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS}))
    {
        std::shared_ptr<Token> op = previous();
        std::shared_ptr<Expression> right = unary();
        return std::make_shared<UnaryExpression>(op, right);
    }

    return primary();
}

std::shared_ptr<Expression> Parser::primary()
{
    if (match({TokenType::JSFALSE}))
    {
        return std::make_shared<BooleanLiteral>(false);
    }
    else if (match({TokenType::JSTRUE}))
    {
        return std::make_shared<BooleanLiteral>(true);
    }
    else if (match({TokenType::JSNULL}))
    {
        // FIXME: This should not a string...
        return std::make_shared<StringLiteral>("null");
    }
    else if (match({TokenType::UNDEFINED}))
    {
        // FIXME: This should not a string...
        return std::make_shared<StringLiteral>("undefined");
    }

    if (match({TokenType::NUMBER}))
    {
        return std::make_shared<NumericLiteral>(std::stod(previous()->value));
    }

    if (match({TokenType::STRING}))
    {
        return std::make_shared<StringLiteral>(previous()->value);
    }

    if (match({TokenType::IDENTIFIER}))
    {
        std::shared_ptr<Value> value;
        for (auto record: m_variable_enviroment->m_dec_env_records)
        {
            if (record->m_record.find(previous()->lexeme) != record->m_record.end())
            {
                value = record->m_record.find(previous()->lexeme)->second;
                break;
            }

        }
        return std::make_shared<VariableExpression>(value);
    }

    if (match({TokenType::LEFT_PAREN}))
    {
        std::shared_ptr<Expression> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<ExpressionStatement>(expr);
    }

    throw error(peek(), "Expect expression.");
};

std::shared_ptr<Expression> Parser::varDeclaration()
{
    std::shared_ptr<Token> name = consume(TokenType::IDENTIFIER, "Expect variable name");

    std::shared_ptr<Expression> expr;
    if (match({TokenType::EQUAL}))
    {
        expr = expression();
    }

    return std::make_shared<VariableDeclaration>(name->lexeme, expr, m_variable_enviroment);
}

std::shared_ptr<Token> Parser::consume(TokenType type, std::string message)
{
    if (check(type))
        return advance();
    throw error(peek(), message);

}

std::unique_ptr<SyntaxError> Parser::error(std::shared_ptr<Token> token, std::string message)
{
    js.error(token, message);
    return std::make_unique<SyntaxError>("Syntax Error", message);
}

void Parser::synchronize()
{
    advance();

    while (!isAtEnd())
    {
        if (previous()->type == TokenType::SEMICOLON)
            return;

        switch (peek()->type)
        {
            case TokenType::FUNCTION:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::RETURN:
                return;
        }

        advance();
    }
}


bool Parser::check(TokenType type)
{
    if (isAtEnd())
        return false;
    return peek()->type == type;
}

bool Parser::match(std::vector<TokenType> types)
{
    for (auto type: types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }

    return false;
}

std::shared_ptr<Token> Parser::advance()
{
    if (!isAtEnd())
        currentParserToken++;
    return previous();
}

bool Parser::isAtEnd()
{
    return peek()->type == TokenType::ENDOFFILE;
}

std::shared_ptr<Token> Parser::peek()
{
    return tokens[currentParserToken];
}

std::shared_ptr<Token> Parser::previous()
{
    return tokens[currentParserToken - 1];
}
