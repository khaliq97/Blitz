#ifndef NAMEDNODEMAP_H
#define NAMEDNODEMAP_H
#include <DOM/Attribute.h>
#include <vector>
class NamedNodeMap
{
	public:
		int length;

		std::vector<Attribute> attributes;
		int getLength() { return attributes.size(); }
		Attribute item(int index)
		{
			//if (index >= attributes.size())
					//return 0;
			return attributes[index];
		}

		Attribute getNamedItem(std::string qualifiedName)
		{
			for (auto attribute: attributes)
			{
				if (attribute.name == qualifiedName)
				{
					return attribute;
				}
			}
		}
};

#endif // NAMEDNODEMAP_H
