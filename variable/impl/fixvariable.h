#ifndef FIXVARIABLE_H
#define FIXVARIABLE_H

#include <utility>
#include <vector>

#include <QHash>

#include "variablebase.h"


class FixVariable : public VariableBase
{
public:
    virtual ~FixVariable();

    virtual QJsonObject toJson() override;
    virtual void fromJson(const QJsonObject& root) override;
    virtual bool replace(const QString& key, QString& val) override;

    void setVariables(const std::vector<std::pair<QString, QString>>& vars);

private:
    QHash<QString, QString> varMap;
};

#endif // FIXVARIABLE_H
