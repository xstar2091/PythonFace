#ifndef VARIABLEMANAGER_H
#define VARIABLEMANAGER_H

#include <utility>
#include <vector>

#include <QString>

#include "impl/variablebase.h"

class VariableManagerImpl;

class VariableManager
{
public:
    VariableManager();
    ~VariableManager();

public:
    ///
    /// \brief 变量替换.
    ///
    /// 替换顺序：全局自动变量->全局变量->局部变量
    /// 局部变量优先级最高。若一个变量同时存在于三种变量管理器中，
    /// 则以局部变量为准，另外两种会被覆盖。
    /// \param source 源字符串
    /// \return 替换后的字符串
    ///
    QString replace(const QString& source);
    VariableBase* getLocalFixVariables();
    void setLocalFixVariables(const std::vector<std::pair<QString, QString>>& vars);
    void setGlobalFixVariables(const std::vector<std::pair<QString, QString>>& vars);

private:
    VariableManagerImpl* impl;
};

#endif // VARIABLEMANAGER_H
