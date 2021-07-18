#include "variablemanagerimpl.h"

#include "autovariable.h"

VariableManagerImpl::VariableManagerImpl()
{
    variableList.push_back(&localFixVar);
    variableList.push_back(&globalFixVar);
    variableList.push_back(AutoVariable::Instance());
}

bool VariableManagerImpl::isVariable(const QString &str)
{
    return str.startsWith("${") && str.endsWith("}");
}

std::vector<QString> VariableManagerImpl::split(const QString &source)
{
    std::vector<QString> blocks;
    int start = source.indexOf("${");
    if (start < 0) {
        blocks.push_back(source);
        return blocks;
    }
    if (start > 0) {
        blocks.push_back(source.mid(0, start));
    }
    int end = source.indexOf("}", start);
    if (end < 0) {
        blocks.push_back(source.mid(start));
        return blocks;
    }

    while (start >= 0 && end > 0) {
        end++;
        blocks.push_back(source.mid(start, end - start));
        start = source.indexOf("${", end);
        if (start < 0) {
            blocks.push_back(source.mid(end));
            return blocks;
        }
        blocks.push_back(source.mid(end, start - end));
        end = source.indexOf("}", start);
        if (end < 0) {
            blocks.push_back(source.mid(start));
            return blocks;
        }
    }

    return blocks;
}

VariableManagerImpl::~VariableManagerImpl()
{

}

VariableBase *VariableManagerImpl::getLocalFixVariables()
{
    return &localFixVar;
}

QString VariableManagerImpl::replace(const QString &source)
{
    QString dest;
    std::vector<QString> blocks = this->split(source);
    for (QString& key : blocks) {
        if (this->isVariable(key)) {
            QString val;
            for (VariableBase* base : variableList) {
                if (base->replace(key, val)) {
                    key = val;
                    break;
                }
            }
            if (val.isEmpty()) {
                dest.append(key);
            }
            else {
                dest.append(val);
            }
        }
        else {
            dest.append(key);
        }
    }
    return dest;
}

void VariableManagerImpl::setLocalFixVariables(const std::vector<std::pair<QString, QString> > &vars)
{
    localFixVar.setVariables(vars);
}

void VariableManagerImpl::setGlobalFixVariables(const std::vector<std::pair<QString, QString> > &vars)
{
    globalFixVar.setVariables(vars);
}
