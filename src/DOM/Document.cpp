#include <stdio.h>
#include <vector>
#include <DOM/Document.h>

//std::vector<std::shared_ptr<Node>> childNodes;
Document::Document()
{

}

Document::Document(std::shared_ptr<Node> parentNode) : Node(parentNode)
{

}
