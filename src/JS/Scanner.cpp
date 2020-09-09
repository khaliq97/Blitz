#include <JS/Scanner.h>
#include <JS/JS.h>
#include <map>
#include <iostream>
int start;
int current;
int line;

static JS js;

std::map<std::string, TokenType> keywords =
{
    {"and", TokenType::AND},
    {"else", TokenType::ELSE},
    {"false", TokenType::JSFALSE},
    {"for", TokenType::FOR},
    {"function", TokenType::FUNCTION},
    {"if", TokenType::IF},
    {"null", TokenType::JSNULL},
    {"undefined", TokenType::UNDEFINED},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"this", TokenType::THIS},
    {"true", TokenType::JSTRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE},

};

Scanner::Scanner(std::string source)
{
    this->source = source;
    start = 0;
    current = 0;
    line = 1;
}

std::vector<std::shared_ptr<Token>> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }
    std::shared_ptr<Value> value = std::make_shared<Value>("");
    std::shared_ptr<Token> token = std::make_shared<Token>(TokenType::ENDOFFILE, "", value, line);
    tokens.push_back(std::move(token));
    return tokens;
}

void Scanner::scanToken()
{
    char c = advance();

    switch (c)
    {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;
        case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
        case '/':
            if (match('/'))
            {
                while(peek() != '\n' && !isAtEnd())
                {
                    advance();
                }
            }
            else
            {
                addToken(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            line++;
            break;
        case '"':
            string();
            break;

        default:
            if (isDigit(c))
            {
                number();
            }
            else if (std::isalpha(c))
            {
                identifier();
            }
            else
            {
                js.error(line, "Unexpected character.\n");
            }

            break;
    }
}

void Scanner::identifier()
{
    while(std::isalpha(peek()))
        advance();
    std::string text = source.substr(start, current - start);

    std::map<std::string, TokenType>::iterator it;

    it = keywords.find(text);
    if (it != keywords.end())
    {
        addToken(keywords[text]);
    }
    else
    {
        addToken(TokenType::IDENTIFIER);
    }

}

void Scanner::number()
{
    while (isDigit(peek()))
        advance();

    if (peek() == '.' && isDigit(peekNext()))
    {
        advance();

        while (isDigit(peek()))
            advance();
    }

    std::shared_ptr<Value> value = std::make_shared<Value>(std::stod(source.substr(start, current - start)));
    addToken(TokenType::NUMBER, value);
}

void Scanner::string()
{
    while(peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            line++;
        advance();
    }

    if (isAtEnd())
    {
        js.error(line, "Unterminated string.");
        return;
    }

    advance();

    std::shared_ptr<Value> value = std::make_shared<Value>(source.substr(start + 1, current - start - 2));
    addToken(TokenType::STRING, value);
}

bool Scanner::match(char expected)
{
    if(isAtEnd())
        return false;
    if (source.at(current) != expected)
        return false;

    current++;
    return true;
}

char Scanner::peek()
{
    if (isAtEnd())
        return '\0';
    return source.at(current);
}

char Scanner::peekNext()
{
    if (current + 1 >= source.length()) return '\0';
    return source.at(current + 1);
}

bool Scanner::isDigit(char c )
{
    return c >= '0' && c <= '9';
}
void Scanner::addToken(TokenType type)
{
    std::shared_ptr<Value> value = std::make_shared<Value>("");
    addToken(type, value);
}

void Scanner::addToken(TokenType type, std::shared_ptr<Value> literal)
{
    std::string text = source.substr(start, current - start);
    std::shared_ptr<Token> token = std::make_shared<Token>(type, text, literal, line);
    tokens.push_back(std::move(token));
}

bool Scanner::isAtEnd()
{
    return current >= source.length();
}

char Scanner::advance()
{
    current++;
    return source.at(current - 1);
}
