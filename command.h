#ifndef COMMAND_H
#define COMMAND_H

#include <QIODevice>
#include <QString>

class Command
{
public:
    Command(QString dir, QString cmd);
    ~Command();

public:
    Command* appendCommand(QString dir, QString cmd);
    Command* getLastCommand();
    void prepareCommdandString();
    QString getPreparedCommandString();
    QString getStdoutString();
    void run();

private:
    /*
    **目录：C:\Users\liuxi\PycharmProjects\learn\
    **命令：main.py "1 11" 2 > 1.txt
    **
    **cmd_text_: main.py "1 11" 2 > 1.txt
    **cmd_str_: main.py "1 11" 2
    ****dir_str_: C:\Users\liuxi\PycharmProjects\learn\
    **redirect_file_name_: 1.txt
    **stdout_str_: main.py的输出
    **
    **dir_str_最后的斜杠有无均可
    **重定向符号支持>和>>
    */
    bool is_prepared_;
    QString cmd_text_;
    QString cmd_str_;
    QString dir_str_;
    QString redirect_file_name_;
    QString stdout_str_;
    QIODevice::OpenMode redirect_file_open_mode_;
    Command* next_cmd_;
};

#endif // COMMAND_H
