#include "commandrunner.h"

CommandRunner::CommandRunner()
{
    cmd_ = new Command("", "");
}

CommandRunner::~CommandRunner()
{
    delete cmd_;
}

Command *CommandRunner::appendCommand(QString dir, QString cmd)
{
    return cmd_->getLastCommand()->appendCommand(dir, cmd);
}

void CommandRunner::clean()
{
    delete cmd_;
    cmd_ = new Command("", "");
}

QString CommandRunner::getStdoutString()
{
    return cmd_->getLastCommand()->getStdoutString();
}

void CommandRunner::run()
{
    cmd_->run();
}
