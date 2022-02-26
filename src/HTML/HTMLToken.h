#ifndef HTMLTOKEN_H
#define HTMLTOKEN_H
#include <string>
#include <memory>
#include <vector>
class HTMLToken
{
    friend class HTMLDocumentParser;
    public:
        HTMLToken();
        enum Type
        {
            Doctype,
            StartTag,
            EndTag,
            Comment,
            Character,
            EndOfFile
        };

        struct Attribute
        {
           std::string name;
           std::string value;
        };

        struct
        {
            std::string name;
            bool isSelfClosing;
            std::vector<Attribute> attributes;
        } tag;

        Type getType() const { return this->type; }
        void setType(Type type) { this->type = type; }

        bool isDoctypeToken() const { return type == Type::Doctype; }
        bool isStartTagToken() const { return getType() == Type::StartTag; }
        bool isEndTagToken() const { return getType() == Type::EndTag; }
        bool isCommentToken() const { return getType() == Type::Comment; }
        bool isCharacterToken() const { return getType() == Type::Character; }
        bool isEndOfFileToken() const { return getType() == Type::EndOfFile; }

        std::string getCommentOrCharacterData() const;
        void appendCommentOrCharacterData(char data) { this->commentOrCharacter.data.push_back(data); }

        std::string getDoctypeName() const { return doctype.name; }
        std::string getPublicIdentifier() const { return doctype.publicIdentifier; }
        std::string getSystemIdentifier() const { return doctype.systemIdentifier; }

        void appendDoctypeName(char data) { this->doctype.name.push_back(data); }

        std::string getTagName() const { return tag.name; }
        void appendTagName(char data) { this->tag.name.push_back(data); }
        bool getTagIsSelfClosing() const { return tag.isSelfClosing; }
        void setTagIsSelfClosing(bool val) {this->tag.isSelfClosing = val; }

        bool isParserWhitespace() const
        {
            if (!isCharacterToken())
                return false;

            if (commentOrCharacter.data == "\t" || commentOrCharacter.data == "\n" || commentOrCharacter.data == " ")
                return true;

            return false;
        }

    private:
        Type type;

        struct
        {
            std::string name;
            std::string publicIdentifier;
            std::string systemIdentifier;
            bool forceQuirks;
        } doctype;

        struct
        {
            std::string data;
        } commentOrCharacter;
};

#endif
