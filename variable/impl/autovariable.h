#ifndef AUTOVARIABLE_H
#define AUTOVARIABLE_H

#include "variablebase.h"


class AutoVariable : public VariableBase
{
public:
    static AutoVariable* Instance();
    virtual ~AutoVariable();
    virtual bool replace(const QString& key, QString& val) override;

private:
    AutoVariable();
    AutoVariable(AutoVariable&&) = delete;
    AutoVariable(const AutoVariable&) = delete;
    AutoVariable& operator=(AutoVariable&&) = delete;
    AutoVariable& operator=(const AutoVariable&) = delete;
};

#endif // AUTOVARIABLE_H
