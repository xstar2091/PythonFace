#include "variablemanager.h"

#include "variable/impl/variablemanagerimpl.h"

VariableManager::VariableManager()
{
    impl = new VariableManagerImpl();
}

VariableManager::~VariableManager()
{
    delete impl;
    impl = nullptr;
}

QString VariableManager::replace(const QString &source)
{
    return impl->replace(source);
}

VariableBase *VariableManager::getLocalFixVariables()
{
    return impl->getLocalFixVariables();
}

void VariableManager::setLocalFixVariables(const std::vector<std::pair<QString, QString> > &vars)
{
    impl->setLocalFixVariables(vars);
}

void VariableManager::setGlobalFixVariables(const std::vector<std::pair<QString, QString> > &vars)
{
    impl->setGlobalFixVariables(vars);
}
