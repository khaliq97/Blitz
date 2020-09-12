#ifndef VALUE_H
#define VALUE_H
#include <string>
#include <sstream>
class Value
{
    public:
        Value() { }
        enum ValueType
        {
            NUMBER,
            BOOL,
            STRING,
            UNDEFINED
        };

        Value(ValueType type)
        {
            this->type = type;
        }

        Value(double value)
        {
            m_value.asDouble = value;
            type = ValueType::NUMBER;
        }
        Value(bool value)
        {
            m_value.asBool = value;
            type = ValueType::BOOL;
        }
        Value(std::string value)
        {
            m_value.asString = value;
            type = ValueType::STRING;
        }

        std::string toString()
        {
            std::string toStringReturn;
            switch (type)
            {
                case ValueType::NUMBER: {
                    std::ostringstream strs;
                    strs << m_value.asDouble;
                    return strs.str();
                }
                case ValueType::STRING: {
                     return m_value.asString;
                }
                case ValueType::BOOL: {
                    return m_value.asBool ? "true" : "false";
                }
                case ValueType::UNDEFINED: {
                    return "undefined";
                }
            }
        }

        ValueType type;

        struct {
            bool asBool;
            double asDouble;
            std::string asString;
        } m_value;

    private:

};

#endif // VALUE_H
