#include <Browser/Layout.h>
Layout::Layout()
{
    paddingTop = std::make_unique<Dimension>();
    paddingBottom = std::make_unique<Dimension>();
    paddingLeft = std::make_unique<Dimension>();
    paddingRight = std::make_unique<Dimension>();

    marginTop = std::make_unique<Dimension>();
    marginBottom = std::make_unique<Dimension>();
    marginLeft = std::make_unique<Dimension>();
    marginRight = std::make_unique<Dimension>();

    borderThickness = std::make_unique<Dimension>();
    fontSize = std::make_unique<Dimension>();
    display = "";

}
