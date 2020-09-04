#ifndef COMMENT_H
#define COMMENT_H
#include <DOM/CharacterData.h>

class Comment : public CharacterData
{
    public:
        Comment(std::shared_ptr<Node> parentNode);
};

#endif // COMMENT_H
