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
    SimpleSelector(const std::shared_ptr<CSSToken>& identToken);
    SelectorType type;
    virtual ~SimpleSelector();
    std::shared_ptr<CSSToken> identToken;
};

class TypeSelector : public SimpleSelector
{
public:
    TypeSelector(const std::shared_ptr<CSSToken>& identToken) : SimpleSelector(identToken)
    {
        type = SelectorType::Type;
    }

    class NSPrefix
    {
    public:
        char asterisk;
        std::shared_ptr<CSSToken> identToken;
    } nsPrefix;
};

class IdSelector : public SimpleSelector
{
public:
    IdSelector(const std::shared_ptr<CSSToken>& hashToken) : SimpleSelector(hashToken)
    {
        type = SelectorType::Id;
    }
};

class ClassSelector : public SimpleSelector
{
public:
    ClassSelector(const std::shared_ptr<CSSToken>& identToken) : SimpleSelector(identToken)
    {
        type = SelectorType::Class;
    }
};

class UniversalSelector : public TypeSelector
{
public:
    UniversalSelector(const std::shared_ptr<CSSToken>& identToken) : TypeSelector(identToken)
    {
        type = SelectorType::Universal;
    }

};

#endif // SIMPLESELECTOR_H
