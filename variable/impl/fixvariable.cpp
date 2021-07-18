#include "fixvariable.h"

FixVariable::~FixVariable()
{

}

QJsonObject FixVariable::toJson()
{
    QJsonObject root;
    auto it = varMap.begin();
    for (; it != varMap.end(); ++it) {
        root.insert(it.key(), it.value());
    }
    return root;
}

void FixVariable::fromJson(const QJsonObject &root)
{
    varMap.clear();
    QJsonObject::const_iterator it = root.begin();
    for (; it != root.end(); ++it) {
        varMap[it.key()] = it.value().toString();
    }
}

bool FixVariable::replace(const QString &key, QString &val)
{
    bool found = false;
    auto it = varMap.find(key);
    if (it != varMap.end()) {
        found = true;
        val = it.value();
    }
    return found;
}

void FixVariable::setVariables(const std::vector<std::pair<QString, QString> > &vars)
{
    varMap.clear();
    for (const auto& pair : vars) {
        varMap[pair.first] = pair.second;
    }
}
