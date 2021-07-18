#ifndef VARIABLEMANAGERIMPL_H
#define VARIABLEMANAGERIMPL_H

#include <vector>

#include <QString>

#include "fixvariable.h"


class VariableManagerImpl
{
public:
    VariableManagerImpl();
    ~VariableManagerImpl();

public:
    VariableBase* getLocalFixVariables();
    QString replace(const QString& source);
    void setLocalFixVariables(const std::vector<std::pair<QString, QString>>& vars);
    void setGlobalFixVariables(const std::vector<std::pair<QString, QString>>& vars);

private:
    VariableManagerImpl(VariableManagerImpl&&) = delete;
    VariableManagerImpl(const VariableManagerImpl&) = delete;
    VariableManagerImpl& operator=(VariableManagerImpl&&) = delete;
    VariableManagerImpl& operator=(const VariableManagerImpl&) = delete;

    bool isVariable(const QString& str);

    ///
    /// \brief 将源字符串按照变量进行切割
    ///
    /// 若源字符串为：python my.py ${Year} 1.txt ${Month} 2.txt
    /// 则切割为5部分
    /// 1: "python my.py "
    /// 2: "${Year}"
    /// 3: " 1.txt "
    /// 4: "${Month}"
    /// 5: " 2.txt"
    /// \param source 待替换的字符串
    /// \return 切割后的vector
    ///
    std::vector<QString> split(const QString& source);

private:
    FixVariable localFixVar;
    FixVariable globalFixVar;
    std::vector<VariableBase*> variableList;
};

#endif // VARIABLEMANAGERIMPL_H
