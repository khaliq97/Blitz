#ifndef VARIABLEENVIROMENT_H
#define VARIABLEENVIROMENT_H
#include <JS/Enviroment/DeclarativeEnviromentRecord.h>
#include <memory>
#include <vector>
class VariableEnviroment {
public:

    VariableEnviroment();

    std::vector<std::shared_ptr<DeclarativeEnviromentRecord>> dec_env_records() { return m_dec_env_records; }
    std::string to_string();
    std::vector<std::shared_ptr<DeclarativeEnviromentRecord>> m_dec_env_records;

private:


};
#endif // VARIABLEENVIROMENT_H
