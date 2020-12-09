#include <CSS/Tokenizer.h>

Tokenizer::Tokenizer(std::string css)
{
    this->source = css;
    current = 0;
    scanTokens();
}

void Tokenizer::scanTokens()
{
    while (!isAtEnd())
    {
        scanToken();
    }

    addToken(CSSTokenType::EndOfFile);

    printf("Tokens: \n");
    for (auto token: tokens)
    {
        if (token->type == CSSTokenType::Whitespace)
        {
            continue;
        }
        printf("%s\n", CSSTokenTypes[(int) token->type].c_str());
        printf("    Value: %s\n", token->value().c_str());

        if (token->type == CSSTokenType::Dimension)
        {
            printf("    Unit: %s\n", token->unit.c_str());
        }

        if (token->type == CSSTokenType::Number)
        {
            printf("    Number Repr: %s\n", token->value().c_str());
            printf("    Number Type: %d\n", (int) token->numberType());
        }
    }

}

void Tokenizer::scanToken()
{
    char c = peek();
    //printf("Current token: %c\n", peek());
    switch (c)
    {
        case '/':
            if (peekNext() == '*')
            {
                printf("Comment!\n");
                consumeSpecific('/');
                consumeSpecific('*');

                while (peek() != '*' && peekNext() != '/')
                {
                    consume();
                }

                consumeSpecific('*');
                consumeSpecific('/');
            }
            break;
        case '\n':
        case '\r':
        case '\t':
        case ' ':
            consume();
            addToken(CSSTokenType::Whitespace);
            break;
       case '\"':
            consumeSpecific('\"');
            string();
            break;
       case '#':
            consumeSpecific('#');
            if (isNameCodePoint(peek())) //TODO: Check if next two code points are a valid escape
            {
                hash();
            }
            break;
       case '\'':
            notSupported("Single quote strings");
       case '(':
            consumeSpecific('(');
            addToken(CSSTokenType::LeftParenthesis);
            break;
        case ')':
            consumeSpecific(')');
            addToken(CSSTokenType::RightParenthesis);
            break;
        case '+':
            if (wouldStartNumber(peek(), peekNext(), peekAtOffset(2)))
            {
                consume();
                tokens.push_back(createNumericToken());
            }
            else
            {
                std::shared_ptr<CSSToken> delimToken = std::make_shared<CSSToken>(CSSTokenType::Delim);
                delimToken->appendValue(peek());
                consume();
            }
            break;
        case ',':
            consume();
            addToken(CSSTokenType::Comma);
            break;
        case '-':
            notSupported("Hyphen minus");
        case '.':
            if (wouldStartNumber(peek(), peekNext(), peekAtOffset(2)))
            {
                consume();
                tokens.push_back(createNumericToken());
            }
            else
            {
                std::shared_ptr<CSSToken> delimToken = std::make_shared<CSSToken>(CSSTokenType::Delim);
                delimToken->appendValue(peek());
                consume();
                tokens.push_back(delimToken);
            }
            break;
        case ':':
            addToken(CSSTokenType::Colon);
            consume();
            break;
        case ';':
            addToken(CSSTokenType::Semicolon);
            consume();
            break;
        case '<':
            notSupported("Less than signs");
        case '@':
            notSupported("Commercial At (@)");
        case '[':
            addToken(CSSTokenType::LeftSquareBracket);
            consume();
            break;
        case '\\':
            notSupported("Reverse solidus");
        case ']':
            addToken(CSSTokenType::RightSquareBracket);
            consume();
            break;
        case '{':
            addToken(CSSTokenType::LeftCurlyBracket);
            consume();
            break;
        case '}':
            addToken(CSSTokenType::RightCurlyBracket);
            consume();
            break;
        default:
            if (std::isdigit(peek()))
            {
                tokens.push_back(createNumericToken());
            }
            else if (isNameStartCodePoint(peek()))
            {
                //printf("HERE!\n");
                std::shared_ptr<CSSToken> identLikeToken = std::make_shared<CSSToken>(CSSTokenType::IdentLike);
                //printf("consume name returns: %s\n", consumeName().c_str());
                identLikeToken->setValue(consumeName());
                tokens.push_back(identLikeToken);
            }
            else
            {
                std::shared_ptr<CSSToken> delimToken = std::make_shared<CSSToken>(CSSTokenType::Delim);
                delimToken->appendValue(peek());
                consume();
                tokens.push_back(delimToken);
            }
            break;


    }
}

void Tokenizer::notSupported(std::string tokenName)
{
    printf("%s are not supported :(\n", tokenName.c_str());
    exit(0);
}

bool Tokenizer::wouldStartNumber(char c1, char c2, char c3)
{
    if (c1 == '+' || c1 == '-')
    {
        if (std::isdigit(c2))
        {
            return true;
        }
        else if (c2 == '.' && std::isdigit(c3))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    if (c1 == '.')
    {
        if (std::isdigit(c2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    if (std::isdigit(c1))
        return true;

    return false;
}

bool Tokenizer::wouldStartIdentifier(char c1, char c2, char c3)
{
    if (c1 == '-')
    {
        if (isNameStartCodePoint(c2) || c2 == '-')
        {
            //TODO: Check if the second and third code points are a valid escape
            return true;
        }
    }

    if (isNameStartCodePoint(c1))
    {
        return true;
    }

    //TODO: Check if the first and second code points are a valid escape

    return false;
}

bool Tokenizer::isNameStartCodePoint(char c)
{
    // printf("isAlpha: %c returns %d\n", c, std::isalpha(c));
    return std::isalpha(c) || c == '_'; //TODO: Needs to check for non-ASCII code point (control)
}

bool Tokenizer::isNameCodePoint(char c)
{
    return (isNameStartCodePoint(c)) || std::isdigit(c) || c == '-';
}

std::shared_ptr<CSSToken> Tokenizer::createNumericToken()
{
    std::shared_ptr<Number> result = consumeNumber();
    std::shared_ptr<CSSToken> token;

    if (wouldStartIdentifier(peek(), peekNext(), peekAtOffset(2)))
    {
        token = std::make_shared<CSSToken>(CSSTokenType::Dimension);
        token->setValue(result->repr);
        token->setNumberType(NumberType::Number);

        token->unit = consumeName();

        return token;
    }

    if (peek() == '%')
    {
        consumeSpecific('%');
        token = std::make_shared<CSSToken>(CSSTokenType::Percentage);
        token->setValue(result->repr);

        return token;
    }

    token = std::make_shared<CSSToken>(CSSTokenType::Number);
    token->setValue(result->repr);
    token->setNumberType(result->type);

    return token;
}

const std::shared_ptr<Number> Tokenizer::consumeNumber()
{
    std::shared_ptr<Number> number = std::make_shared<Number>();
    number->type = NumberType::Integer;
    number->repr = "";

    if (peek() == '+' || peek() == '-')
    {
        number->repr += peek();
        consume();
    }

    while (std::isdigit(peek()) && !isAtEnd())
    {
        number->repr += peek();
        consume();
    }

    if (peek() == '.' && std::isdigit(peekNext()))
    {
        number->repr += peek();
        number->repr += peekNext();

        consumeSpecific('.');
        consume();

        number->type = NumberType::Number;
    }

    return number;

    //TODO: 5. If the next 2 or 3 input code points are U+0045 LATIN CAPITAL LETTER E (E)
    // or U+0065 LATIN SMALL LETTER E (e), optionally followed by U+002D HYPHEN-MINUS (-) or U+002B PLUS SIGN

}

std::string Tokenizer::consumeName()
{
    std::string result = "";
    while (!isAtEnd())
    {
        // printf("isNameCodePoint is: %d\n", isNameCodePoint(peek()));
        if (isNameCodePoint(peek()))
        {
            //printf("Result state: %s\n", result.c_str());
            result += peek();
            consume();
        }
        else
        {
            //printf("Returned: %s\n", result.c_str());
            return result;
        }

        //TODO: the stream starts with a valid escape
    }

    return result;
}

void Tokenizer::hash()
{
    std::shared_ptr<CSSToken> token;

    if (isNameCodePoint(peek()))
    {
        token = std::make_shared<CSSToken>(CSSTokenType::Hash);
        if (wouldStartIdentifier(peek(), peekNext(), peekAtOffset(2)))
        {
            token->setHashTokenType(HashType::ID);
        }

        token->setValue(consumeName());
        tokens.push_back(token);
    }
    else
    {
        token = std::make_shared<CSSToken>(CSSTokenType::Delim);
        token->appendValue(peek());
        consume();
    }
}

void Tokenizer::string()
{
    std::shared_ptr<CSSToken> stringToken = std::make_shared<CSSToken>(CSSTokenType::String);
    std::string stringValue = "";
    printf("Consuming string\n");
    while (peek() != '\"')
    {
        if (isAtEnd())
        {
            tokens.push_back(stringToken);
            printf("Parse error!\n");
            exit(0);
        }
        else if (peek() =='\n')
        {
            consumeSpecific('\n');
            tokens.push_back(std::make_shared<CSSToken>(CSSTokenType::BadString));
            printf("Parse error!\n");
            exit(0);
        }
        else
        {
            printf("String consumption yields char: %c\n", peek());
            stringValue += peek();
            consume();
        }
    }
    consumeSpecific('\"');
    tokens.push_back(stringToken);
    printf("Finished consuming string\n");

    //Todo: U+005C REVERSE SOLIDUS (\)

}

void Tokenizer::addToken(CSSTokenType type)
{
    std::shared_ptr<CSSToken> token = std::make_shared<CSSToken>(type);
    tokens.push_back(std::move(token));
}

bool Tokenizer::match(char expected)
{
    if(isAtEnd())
        return false;
    if (source.at(current) != expected)
        return false;

    current++;
    return true;
}


char Tokenizer::peek()
{
    if (current >= source.length())
        return '\0';
    return source.at(current);
}

char Tokenizer::peekAtOffset(int index)
{
    if (current >= source.length())
        return '\0';
    return source.at(current + index);
}

char Tokenizer::peekNext()
{
    if (current + 1 >= source.length()) return '\0';
    return source.at(current + 1);
}

bool Tokenizer::isAtEnd()
{
    return current >= source.length();
}

void Tokenizer::consumeSpecific(char c)
{
    if (!isAtEnd() && source.at(current) == c)
    {
        current++;
    }
}

void Tokenizer::consume()
{
    if(!isAtEnd())
        current++;
}
