#ifndef HTMLTOKEN_H
#define HTMLTOKEN_H
#include <string>
#include <map>
#include <memory>

class HTMLToken
{
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

        Type getType() const { return this->type; }
        void setType(Type type) { this->type = type; }

        bool isDoctypeToken() { return type == Type::Doctype; }
        bool isStartTagToken() { return getType() == Type::StartTag; }
        bool isEndTagToken() { return getType() == Type::EndTag; }
        bool isCommentToken() { return getType() == Type::Comment; }
        bool isCharacterToken() { return getType() == Type::Character; }
        bool isEndOfFileToken() { return getType() == Type::EndOfFile; }

        std::string getCommentOrCharacterData();
        void appendCommentOrCharacterData(char data) { this->commentOrCharacter.data.push_back(data); }

        std::string getDoctypeName() { return doctype.name; }
        std::string getPublicIdentifier() { return doctype.publicIdentifier; }
        std::string getSystemIdentifier() { return doctype.systemIdentifier; }

        void appendDoctypeName(char data) { this->doctype.name.push_back(data); }

        std::string getTagName() { return tag.name; }
        void appendTagName(char data) { this->tag.name.push_back(data); }
        bool getTagIsSelfClosing() { return tag.isSelfClosing; }
        void setTagIsSelfClosing(bool val) {this->tag.isSelfClosing = val; }

        bool isParserWhitespace()
        {
            if (!isCharacterToken())
                return false;

            if (commentOrCharacter.data == "\t" || commentOrCharacter.data == "\n" || commentOrCharacter.data == " ")
                return true;

            return false;
        }

        std::map<std::string, std::string> *getAttributes() { return this->tag.attributes; }

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
            std::string name;
            bool isSelfClosing;
            std::map<std::string, std::string> *attributes;
        } tag;

        struct
        {
            std::string data;
        } commentOrCharacter;
};

#endif
