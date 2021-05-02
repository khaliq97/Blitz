#ifndef COMMENT_H
#define COMMENT_H
#include <DOM/CharacterData.h>

class Comment : public CharacterData
{
public:
    Comment(std::weak_ptr<Node> parentNode, const std::string commentContent);
};

#endif // COMMENT_H
