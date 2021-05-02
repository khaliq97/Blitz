#ifndef CHARACTERDATA_H
#define CHARACTERDATA_H
#include <DOM/Node.h>
#include <string>
class CharacterData : public Node
{
public:
    CharacterData(std::weak_ptr<Node> parentNode);
    std::string data;
    int length;
    // std::string subStringData(int offset, int count) { }
    void appendData(std::string data);
    // void insertData(int offset, std::string data) { }
    // void deleteData(int offset, int count) { }
    // void replaceData(int offset, int count, std::string data) { }

    // Foster Parenting?
};

#endif // CHARACTERDATA_H
