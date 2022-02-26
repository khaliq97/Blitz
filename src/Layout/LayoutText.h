#ifndef LAYOUT_TEXT_H
#define LAYOUT_TEXT_H
#include <string>
#include <vector>
#include <iostream>
namespace Layout {

class LayoutText {

public:
    LayoutText(std::string text);

    std::string removeLastWord();
    void appendWordToBeginning(const std::string word);
    std::string displayText() const;

private:
    std::vector<std::string> m_words;
};

}
#endif // LAYOUT_TEXT_H
