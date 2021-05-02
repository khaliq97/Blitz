#ifndef CSSTOKEN_H
#define CSSTOKEN_H
#include <string>
enum class CSSTokenType
{
    Whitespace,
    String,
    BadString,
    Hash,
    Delim,
    LeftParenthesis,
    RightParenthesis,
    Comma,
    CDC,
    Colon,
    Semicolon,
    LeftSquareBracket,
    RightSquareBracket,
    LeftCurlyBracket,
    RightCurlyBracket,
    Number,
    Percentage,
    Dimension,
    IdentLike,
    EndOfFile
};

enum class HashType
{
    ID,
    Unrestricted
};

enum class NumberType
{
    Integer,
    Number
};

class Number
{
public:
    std::string repr;
    NumberType type;
};

static std::string CSSTokenTypes[] =
{
    "Whitespace",
    "String",
    "BadString",
    "Hash",
    "Delim",
    "LeftParenthesis",
    "RightParenthesis",
    "Comma",
    "CDC",
    "Colon",
    "Semicolon",
    "LeftSquareBracket",
    "RightSquareBracket",
    "LeftCurlyBracket",
    "RightCurlyBracket",
    "Number",
    "Percentage",
    "Dimension",
    "IdentLike",
    "EndOfFile"
};


class CSSToken
{
public:

    CSSToken() { }
    CSSToken(CSSTokenType type) : type(type)
    {
        if (type == CSSTokenType::Hash)
            this->m_hashType = HashType::Unrestricted;
    }

    CSSTokenType type;
    std::string unit;

    void setValue(std::string value)
    {
        this->m_value = value;
    }

    void appendValue(std::string value)
    {
        this->m_value += value;
    }

    void appendValue(char value)
    {
        this->m_value += value;
    }

    std::string value()
    {
        return m_value;
    }

    void setHashTokenType(HashType value)
    {
        this->m_hashType = value;
    }

    HashType hashType()
    {
        return m_hashType;
    }

    void setNumberType(NumberType value)
    {
        this->m_numberType = value;
    }

    NumberType numberType()
    {
        return m_numberType;
    }

    double valueAsDouble()
    {
        return std::stod(value());
    }

private:
    std::string m_value = "";
    HashType m_hashType;
    NumberType m_numberType;
};

#endif // CSSTOKEN_H
