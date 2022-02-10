#ifndef BLOCKFORMATTINGCONTEXT_H
#define BLOCKFORMATTINGCONTEXT_H
#include <Layout/LayoutBox.h>
class BlockFormattingContext {

public:
    BlockFormattingContext() { }
    void run(std::shared_ptr<LayoutBox>& layoutBox);
    void runForInitialContainingBlock(std::shared_ptr<LayoutBox> &layoutBox, double preDefinedWidth);
private:
    double computeMarginBoxWidth(std::shared_ptr<LayoutBox> &layoutBox);
    double computeBorderBoxWidth(std::shared_ptr<LayoutBox> &layoutBox);
};

#endif // BLOCKFORMATTINGCONTEXT_H
