#include "command.h"

#include <QProcess>

namespace {

QString variableNamePyPath = "${PyPath}";

}

Command::Command(QString dir, QString cmd)
{
    is_prepared_ = false;
    cmd_text_ = cmd;
    dir_str_ = dir;
    next_cmd_ = nullptr;
}

Command::~Command()
{
    delete next_cmd_;
}

Command *Command::appendCommand(QString dir, QString cmd)
{
    if (next_cmd_) {
        return this->getLastCommand()->appendCommand(dir, cmd);
    }
    next_cmd_ = new Command(dir, cmd);
    return next_cmd_;
}

Command *Command::getLastCommand()
{
    Command* cmd = this;
    while (cmd->next_cmd_) {
        cmd = cmd->next_cmd_;
    }
    return cmd;
}

void Command::prepareCommdandString()
{
    if (is_prepared_) {
        return;
    }
    if (cmd_text_.isEmpty()) {
        return;
    }
    int py_index = cmd_text_.indexOf(".py");//.py index
    if (py_index <= 0) {
        return;
    }

    is_prepared_ = true;
    redirect_file_open_mode_ = QIODevice::Append;

    int slash_index = cmd_text_.lastIndexOf("/", py_index);// / index
    if (slash_index < 0) {
        slash_index = cmd_text_.lastIndexOf("\\", py_index);// \ index
        if (slash_index == 0) {
            slash_index = -1;
        }
    }
    if (slash_index >= 0) {
        dir_str_ = cmd_text_.left(slash_index);
        cmd_str_ = cmd_text_.right(cmd_text_.length() - slash_index - 1);
    }
    else {
        if (dir_str_.isEmpty()) {
            dir_str_ = ".";
        }
        cmd_str_ = cmd_text_;
    }

    int redirect_index = cmd_str_.indexOf(">>");
    if (redirect_index > 0) {
        redirect_file_name_ = cmd_str_.right(cmd_str_.length() - redirect_index - 2);
        redirect_file_open_mode_ = QIODevice::Append;
    }
    else {
        redirect_index = cmd_str_.indexOf(">");
        if (redirect_index > 0) {
            redirect_file_name_ = cmd_str_.right(cmd_str_.length() - redirect_index - 1);
            redirect_file_open_mode_ = QIODevice::Truncate;
        }
    }

    if (!redirect_file_name_.isEmpty()) {
        if (redirect_file_name_[0] == '>') {
            redirect_file_name_ = "";
        }
    }
    cmd_str_ = cmd_str_.left(redirect_index).trimmed();
    redirect_file_name_ = redirect_file_name_.trimmed();

    cmd_str_ = cmd_str_.replace(variableNamePyPath, dir_str_);
    redirect_file_name_ = redirect_file_name_.replace(variableNamePyPath, dir_str_);
}

QString Command::getPreparedCommandString()
{
    prepareCommdandString();
    if (!is_prepared_) {
        return QString();
    }
    return QString("python %1/%2").arg(dir_str_, cmd_str_);
}

QString Command::getStdoutString()
{
    if (next_cmd_) {
        return this->getLastCommand()->getStdoutString();
    }
    return stdout_str_;
}

void Command::run()
{
    QString cmd_str = getPreparedCommandString();
    if (!cmd_str.isEmpty() && is_prepared_) {
        QProcess process;
        if (!redirect_file_name_.isEmpty()) {
            process.setStandardOutputFile(redirect_file_name_, redirect_file_open_mode_);
        }
        process.start(cmd_str);
        process.waitForFinished();
        stdout_str_ = process.readAll().toStdString().c_str();
    }
    if (next_cmd_) {
        next_cmd_->run();
    }
}
