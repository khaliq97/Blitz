#include <JS/Error.h>
Error::Error(std::string name, std::string message)
    : name(name), message(message) {}

SyntaxError::SyntaxError(std::string name, std::string message) : Error(name, message)
{

}
