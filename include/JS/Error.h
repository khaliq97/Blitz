#ifndef ERROR_H
#define ERROR_H
#include <string>
#include <exception>
class Error : public std::exception
{
    public:
        Error(std::string name, std::string message);
        std::string name;
        std::string message;


};

class SyntaxError : public Error
{
    public:
        SyntaxError(std::string name, std::string message);
};

#endif // ERROR_H
