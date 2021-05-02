#include <DOM/Comment.h>

Comment::Comment(std::weak_ptr<Node> parentNode, const std::string commentContent) : CharacterData(parentNode)
{
    data = commentContent;
    nodeName = "Comment";
    nodeType = Node::NodeType::CommentNode;
}
