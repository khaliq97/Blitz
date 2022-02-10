#include <Layout/BlockFormattingContext.h>
#include <fmt/core.h>
void BlockFormattingContext::run(std::shared_ptr<LayoutBox> &layoutBox)
{
    if (layoutBox->isBlockBox())
    {
        layoutBox->marginRect.width = computeMarginBoxWidth(layoutBox);

        layoutBox->borderRect.width = computeBorderBoxWidth(layoutBox);

        // Add the margin for the layout box's border rectangle
        layoutBox->borderRect.y += layoutBox->marginTop;
        layoutBox->borderRect.x += layoutBox->marginLeft;

        // Add the parent's padding's and border rectangle existing X and Y coords
        layoutBox->borderRect.y += layoutBox->parent.lock()->paddingTop + layoutBox->parent.lock()->borderRect.y + layoutBox->parent.lock()->borderTopWidth;
        layoutBox->borderRect.x += layoutBox->parent.lock()->paddingLeft + layoutBox->parent.lock()->borderRect.x + layoutBox->parent.lock()->borderLeftWidth;

        if (!layoutBox->isFirstLayoutBoxInContainingBlock(layoutBox))
        {
            // If it's not the first block in the containing block then get the Y position of current block
            layoutBox->borderRect.y += layoutBox->calculateYPositionInContainingBlock(layoutBox);
        }

         layoutBox->updateAllParentHeights();
    }

    if (layoutBox->isInlineBox())
    {
        // TODO: Implement inline boxes
        return;
    }
}

void BlockFormattingContext::runForInitialContainingBlock(std::shared_ptr<LayoutBox> &layoutBox, double preDefinedWidth)
{
    if (layoutBox->isBlockBox())
    {
         layoutBox->marginRect.width = preDefinedWidth;
         layoutBox->borderRect.width = preDefinedWidth;
    }
}

double BlockFormattingContext::computeMarginBoxWidth(std::shared_ptr<LayoutBox>& layoutBox)
{
    return layoutBox->parent.lock()->marginRect.width -
           layoutBox->parent.lock()->borderLeftWidth - layoutBox->parent.lock()->borderRightWidth -
           layoutBox->parent.lock()->paddingLeft - layoutBox->parent.lock()->paddingRight -
           layoutBox->parent.lock()->marginLeft - layoutBox->parent.lock()->marginRight;
}

double BlockFormattingContext::computeBorderBoxWidth(std::shared_ptr<LayoutBox>& layoutBox)
{
   return layoutBox->parent.lock()->borderRect.width -
          layoutBox->marginLeft - layoutBox->marginRight -
          layoutBox->parent.lock()->borderLeftWidth - layoutBox->parent.lock()->borderRightWidth -
          layoutBox->parent.lock()->paddingLeft - layoutBox->parent.lock()->paddingRight;
}
