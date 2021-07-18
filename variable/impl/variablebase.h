#ifndef VARIABLEBASE_H
#define VARIABLEBASE_H

#include <QJsonObject>
#include <QString>


class VariableBase
{
public:
    VariableBase();
    virtual ~VariableBase();

public:
    virtual QJsonObject toJson();
    virtual void fromJson(const QJsonObject& root);
    virtual bool replace(const QString& key, QString& val) = 0;
};

#endif // VARIABLEBASE_H
