#include <Browser/RenderNode.h>
RenderNode::RenderNode(std::string selector)
{
    styleRules = std::make_shared<StyleRule>(selector);
}

RenderNode::~RenderNode()
{

}

RenderNode::RenderNode(std::shared_ptr<StyleRule> styleRules)
{
    this->styleRules = styleRules;
}
