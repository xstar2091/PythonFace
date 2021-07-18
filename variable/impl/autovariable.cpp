#include "autovariable.h"

AutoVariable::AutoVariable()
{

}

AutoVariable::~AutoVariable()
{

}

AutoVariable *AutoVariable::Instance()
{
    static AutoVariable inst;
    return &inst;
}

bool AutoVariable::replace(const QString &key, QString &val)
{
    return false;
}
