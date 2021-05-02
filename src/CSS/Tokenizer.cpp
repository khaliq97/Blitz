#include <CSS/Tokenizer.h>

Tokenizer::Tokenizer(std::string css)
{
    this->source = css;
    current = 0;
    scanTokens();
}

void Tokenizer::printTokens()
{
    printf("Tokens: \n");
    for (CSSToken token: m_tokens)
    {
        if (token.type == CSSTokenType::Whitespace)
        {
            continue;
        }
        printf("%s\n", CSSTokenTypes[(int) token.type].c_str());
        printf("    Value: %s\n", token.value().c_str());

        if (token.type == CSSTokenType::Dimension)
        {
            printf("    Unit: %s\n", token.unit.c_str());
        }

        if (token.type == CSSTokenType::Number)
        {
            printf("    Number Repr: %s\n", token.value().c_str());
            printf("    Number Type: %d\n", (int) token.numberType());
        }
    }
}

void Tokenizer::scanTokens()
{
    while (!isAtEnd())
    {
        scanToken();
    }

    addToken(CSSTokenType::EndOfFile);

#ifdef TOKEN_STREAM
    printTokens();
#endif
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
                m_tokens.push_back(createNumericToken());
            }
            else
            {
                CSSToken delimToken = CSSToken(CSSTokenType::Delim);
                delimToken.appendValue(peek());
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
                m_tokens.push_back(createNumericToken());
            }
            else
            {
                CSSToken delimToken = CSSToken(CSSTokenType::Delim);
                delimToken.appendValue(peek());
                consume();
                m_tokens.push_back(delimToken);
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
                m_tokens.push_back(createNumericToken());
            }
            else if (isNameStartCodePoint(peek()))
            {
                //printf("HERE!\n");
                CSSToken identLikeToken = CSSToken(CSSTokenType::IdentLike);
                //printf("consume name returns: %s\n", consumeName().c_str());
                identLikeToken.setValue(consumeName());
                m_tokens.push_back(identLikeToken);
            }
            else
            {
                CSSToken delimToken = CSSToken(CSSTokenType::Delim);
                delimToken.appendValue(peek());
                consume();
                m_tokens.push_back(delimToken);
            }
            break;


    }
}

void Tokenizer::notSupported(std::string tokenName)
{
    printf("Tokenizer: %s are not supported :(\n", tokenName.c_str());
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

CSSToken Tokenizer::createNumericToken()
{
    const Number& result = consumeNumber();
    CSSToken token;

    if (wouldStartIdentifier(peek(), peekNext(), peekAtOffset(2)))
    {
        token = CSSToken(CSSTokenType::Dimension);
        token.setValue(result.repr);
        token.setNumberType(NumberType::Number);

        token.unit = consumeName();

        return token;
    }

    if (peek() == '%')
    {
        consumeSpecific('%');
        token = CSSToken(CSSTokenType::Percentage);
        token.setValue(result.repr);

        return token;
    }

    token = CSSToken(CSSTokenType::Number);
    token.setValue(result.repr);
    token.setNumberType(result.type);

    return token;
}

Number Tokenizer::consumeNumber()
{
    Number number;
    number.type = NumberType::Integer;
    number.repr = "";

    if (peek() == '+' || peek() == '-')
    {
        number.repr += peek();
        consume();
    }

    while (std::isdigit(peek()) && !isAtEnd())
    {
        number.repr += peek();
        consume();

        if (peek() == '.' && std::isdigit(peekNext()))
        {
            number.repr += peek();
            number.repr += peekNext();

            consumeSpecific('.');
            consume();

            number.type = NumberType::Number;
        }
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
        if (isNameCodePoint(peek()))
        {
            result += peek();
            consume();
        }
        else
        {
            return result;
        }

        //TODO: the stream starts with a valid escape
    }

    return result;
}

void Tokenizer::hash()
{
    CSSToken token;

    if (isNameCodePoint(peek()))
    {
        token = CSSToken(CSSTokenType::Hash);
        if (wouldStartIdentifier(peek(), peekNext(), peekAtOffset(2)))
        {
            token.setHashTokenType(HashType::ID);
        }

        token.setValue(consumeName());
        m_tokens.push_back(token);
    }
    else
    {
        token = CSSToken(CSSTokenType::Delim);
        token.appendValue(peek());
        consume();
    }
}

void Tokenizer::string()
{
    CSSToken stringToken = CSSToken(CSSTokenType::String);
    std::string stringValue = "";
    while (peek() != '\"')
    {
        if (isAtEnd())
        {
            m_tokens.push_back(stringToken);
            printf("Parse error!\n");
            exit(0);
        }
        else if (peek() =='\n')
        {
            consumeSpecific('\n');
            m_tokens.push_back((CSSTokenType::BadString));
            printf("Parse error!\n");
            exit(0);
        }
        else
        {
            stringValue += peek();
            consume();
        }
    }
    consumeSpecific('\"');
    m_tokens.push_back(stringToken);

    //Todo: U+005C REVERSE SOLIDUS (\)

}

void Tokenizer::addToken(CSSTokenType type)
{
    CSSToken token = CSSToken(type);
    m_tokens.push_back(token);
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
