#ifndef RENDERNODE_H
#define RENDERNODE_H
#include <Parser/StyleRule.h>
#include <memory>
class RenderNode
{
    public:
        RenderNode(std::string selector);
         ~RenderNode();
        RenderNode(std::shared_ptr<StyleRule> styleRules);
        std::shared_ptr<StyleRule> styleRules;
};

#endif // RENDERNODE_H
