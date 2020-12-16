#ifndef SIMPLESELECTOR_H
#define SIMPLESELECTOR_H
#include <CSS/CSSToken.h>
#include <memory>

class SimpleSelector
{
public:
    SimpleSelector();
    virtual ~SimpleSelector();
};

//class SubclassSelector : public SimpleSelector
//{
//public
//};

class TypeSelector : public SimpleSelector
{
public:
    TypeSelector(const std::shared_ptr<CSSToken>& identToken) : identToken(identToken) { }

    class NSPrefix
    {
    public:
        char asterisk;
        std::shared_ptr<CSSToken> identToken;
    } nsPrefix;

    std::shared_ptr<CSSToken> identToken;

};

class IdSelector : public SimpleSelector
{
public:
    IdSelector(const std::shared_ptr<CSSToken>& hashToken) : hashToken(hashToken) { }

    std::shared_ptr<CSSToken> hashToken;

};

class ClassSelector : public SimpleSelector
{
public:
    ClassSelector(const std::shared_ptr<CSSToken>& identToken) : identToken(identToken) { }
    std::shared_ptr<CSSToken> identToken;

};

#endif // SIMPLESELECTOR_H
