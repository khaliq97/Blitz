#include <Layout/LayoutText.h>
#include <fmt/core.h>
Layout::LayoutText::LayoutText(std::string text)
{
    std::string buffer;
    for (int i = 0; i < text.size(); i++)
    {
        if (text[i] == ' ')
        {
            m_words.push_back(buffer);
            buffer.clear();
            continue;
        }
        else
        {
            buffer += text[i];
        }

    }

    m_words.push_back(buffer);

}

std::string Layout::LayoutText::removeLastWord()
{
    std::string lastWord = m_words.back();
    m_words.pop_back();
    return lastWord;
}

void Layout::LayoutText::appendWordToBeginning(const std::string word)
{
    m_words.insert(m_words.begin(), word);
}

std::string Layout::LayoutText::displayText() const
{
    std::string displayText;
    for (auto word: m_words)
    {
        displayText.append(" ");
        displayText.append(word);
    }

     displayText.erase(0, 1);

    return displayText;
}


