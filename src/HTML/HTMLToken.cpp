#include <HTML/HTMLToken.h>

HTMLToken::HTMLToken()
{
}

std::string HTMLToken::getCommentOrCharacterData() const
{
    if (!commentOrCharacter.data.empty())
    {
        return commentOrCharacter.data;
    }

    return "";
}
