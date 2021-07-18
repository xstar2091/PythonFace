#ifndef COMMANDRUNNER_H
#define COMMANDRUNNER_H

#include "command.h"


class CommandRunner
{
public:
    CommandRunner();
    ~CommandRunner();

public:
    Command* appendCommand(QString dir, QString cmd);
    void clean();
    QString getStdoutString();
    void run();

private:
    QString dir_;
    Command* cmd_;
};

#endif // COMMANDRUNNER_H
