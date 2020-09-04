#include <Parser/HTMLToken.h>

HTMLToken::HTMLToken()
{
    this->tag.attributes = new std::map<std::string, std::string>();
}

std::string HTMLToken::getCommentOrCharacterData()
{
    if (!commentOrCharacter.data.empty())
    {
        return commentOrCharacter.data;
    }

    return "";
}
