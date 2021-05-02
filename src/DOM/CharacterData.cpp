#include <DOM/CharacterData.h>

CharacterData::CharacterData(std::weak_ptr<Node> parentNode) : Node(parentNode)
{
    this->data = "";
    this->length = 0;
}

void CharacterData::appendData(std::string data)
{
    if (!this->data.empty())
    {
        this->data += data;
    }
}


