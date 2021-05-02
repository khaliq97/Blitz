#ifndef SIMPLESELECTOR_H
#define SIMPLESELECTOR_H
#include <CSS/CSSToken.h>
#include <memory>

enum class SelectorType
{
    Type,
    Id,
    Universal,
    Class
};

class SimpleSelector
{
public:
    SimpleSelector(std::string identToken, SelectorType type);
    std::string value() { return m_value; }
    SelectorType type() { return m_type; }

    //class NSPrefix
    //{
    //public:
    //    char asterisk;
    //    std::string value;
    //} nsPrefix;


private:
    std::string m_value;
    SelectorType m_type;

};

#endif // SIMPLESELECTOR_H
