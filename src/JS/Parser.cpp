#include <JS/Parser.h>
#include <JS/JS.h>
static JS js;

std::shared_ptr<Expression<Value>> Parser::parse()
{
    try {
        return expression();
    } catch (SyntaxError error) {
        printf("%s\n%s", error.name.c_str(), error.message.c_str());
        return nullptr;
    }
}

std::shared_ptr<Expression<Value>> Parser::expression()
{
    return equality();
}

std::shared_ptr<Expression<Value>> Parser::equality()
{
    std::shared_ptr<Expression<Value>> expr = comparison();
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
         std::shared_ptr<Token> op = previous();
         std::shared_ptr<Expression<Value>> right = comparison();
         expr = std::make_shared<BinaryExpression<Value>>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expression<Value>> Parser::comparison()
{
     std::shared_ptr<Expression<Value>> expr = additon();
     while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
     {
          std::shared_ptr<Token> op = previous();
          std::shared_ptr<Expression<Value>> right = additon();
          expr = std::make_shared<BinaryExpression<Value>>(expr, op, right);
     }

     return expr;
}

std::shared_ptr<Expression<Value>> Parser::additon()
{
     std::shared_ptr<Expression<Value>> expr = multiplication();
     while (match({TokenType::MINUS, TokenType::PLUS}))
     {
          std::shared_ptr<Token> op = previous();
          std::shared_ptr<Expression<Value>> right = multiplication();
          expr = std::make_shared<BinaryExpression<Value>>(expr, op, right);
     }

     return expr;
}

std::shared_ptr<Expression<Value>> Parser::multiplication()
{
    std::shared_ptr<Expression<Value>> expr = unary();
    while (match({TokenType::SLASH, TokenType::STAR}))
    {
         std::shared_ptr<Token> op = previous();
         std::shared_ptr<Expression<Value>> right = unary();
         expr = std::make_shared<BinaryExpression<Value>>(expr, op, right);
    }

    return expr;
}


std::shared_ptr<Expression<Value>> Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS}))
    {
        std::shared_ptr<Token> op = previous();
        std::shared_ptr<Expression<Value>> right = unary();
        return std::make_shared<UnaryExpression<Value>>(op, right);
    }

    return primary();
}

std::shared_ptr<Expression<Value>> Parser::primary()
{
    //std::shared_ptr<Expression<Value>> expression;
    if (match({TokenType::JSFALSE}))
    {
        return std::make_shared<LiteralExpression<Value>>(false);
    }
    else if (match({TokenType::JSTRUE}))
    {
        return std::make_shared<LiteralExpression<Value>>(true);
    }
    else if (match({TokenType::JSNULL}))
    {
        return std::make_shared<LiteralExpression<Value>>("null");
    }
    else if (match({TokenType::UNDEFINED}))
    {
        return std::make_shared<LiteralExpression<Value>>("undefined");
    }

    if (match({TokenType::NUMBER}))
    {
        return std::make_shared<LiteralExpression<Value>>(previous()->literal->m_value.asDouble);
    }

    if (match({TokenType::STRING}))
    {
        return std::make_shared<LiteralExpression<Value>>(previous()->literal->m_value.asString);
    }

    if (match({TokenType::LEFT_PAREN}))
    {
        std::shared_ptr<Expression<Value>> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<ExpressionStatement<Value>>(expr);
    }

    throw error(peek(), "Expect expression.");
};

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
