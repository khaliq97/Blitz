#ifndef DECLARATIVEENVIROMENTRECORD_H
#define DECLARATIVEENVIROMENTRECORD_H
#include <map>
#include <JS/Value.h>
#include <memory>
class DeclarativeEnviromentRecord {
public:
    DeclarativeEnviromentRecord();

    std::map<std::string, std::shared_ptr<Value>> record() { return m_record; }
    std::map<std::string, std::shared_ptr<Value>> m_record;

private:

};
#endif // DECLARATIVEENVIROMENTRECORD_H
