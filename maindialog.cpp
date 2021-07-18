#include "maindialog.h"
#include "ui_maindialog.h"

#include <exception>
#include <stdexcept>

#include <QException>
#include <QFile>
#include <QTextStream>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMimeData>

#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSizePolicy>

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
{
    this->initUi();
    ui->setupUi(this);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::initUi()
{
    Qt::WindowFlags flags = Qt::Dialog |
            Qt::WindowMinimizeButtonHint |
            Qt::WindowMaximizeButtonHint |
            Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    mainVBoxLayout = new QVBoxLayout(this);
    this->setLayout(mainVBoxLayout);

    editHBoxLayout = new QHBoxLayout(this);
    commandLayout = new QGridLayout(this);
    resultEdit = new QTextEdit(this);

    mainVBoxLayout->addLayout(editHBoxLayout);
    mainVBoxLayout->addLayout(commandLayout);
    mainVBoxLayout->addWidget(resultEdit);

    btnOpen = new QPushButton("打开", this);
    btnSave = new QPushButton("保存", this);
    btnPlaceholder = new QPushButton("变量", this);
    btnClean = new QPushButton("清空", this);
    btnRun = new QPushButton("运行", this);

    connect(btnOpen, &QPushButton::clicked, this, &MainDialog::onButtonOpenClicked);
    connect(btnSave, &QPushButton::clicked, this, &MainDialog::onButtonSaveClicked);
    connect(btnPlaceholder, &QPushButton::clicked, this, &MainDialog::onButtonPlaceHolderClicked);
    connect(btnClean, &QPushButton::clicked, this, &MainDialog::onButtonCleanClicked);
    connect(btnRun, &QPushButton::clicked, this, &MainDialog::onButtonRunClicked);

    editHBoxLayout->addStretch();
    editHBoxLayout->addWidget(btnOpen);
    editHBoxLayout->addWidget(btnSave);
    editHBoxLayout->addWidget(btnPlaceholder);
    editHBoxLayout->addWidget(btnClean);
    editHBoxLayout->addWidget(btnRun);

    initCommandDirUi();
    addNewCommandEdit(1);

    placeHolderDlg = new PlaceHolderDialog(this);
    this->setAcceptDrops(true);
}

void MainDialog::initCommandDirUi()
{
    QLabel* label = new QLabel("目录", this);
    QLineEdit* edit = new QLineEdit(this);
    QPushButton* btnRemove = new QPushButton("-", this);
    btnRemove->setMaximumWidth(75);
    connect(btnRemove, &QPushButton::clicked, this, &MainDialog::onButtonRemoveCommandClicked);
    QPushButton* btnAdd = new QPushButton("+", this);
    btnAdd->setMaximumWidth(75);
    connect(btnAdd, &QPushButton::clicked, this, &MainDialog::onButtonAddCommandClicked);

    commandLayout->addWidget(label, 0, 0);
    commandLayout->addWidget(edit, 0, 1);
    commandLayout->addWidget(btnRemove, 0, 2);
    commandLayout->addWidget(btnAdd, 0, 3);
}

void MainDialog::addNewCommandEdit(int row)
{
    QLabel* label = new QLabel("命令", this);
    QLineEdit* edit = new QLineEdit(this);
    QPushButton* btnRemove = new QPushButton("-", this);
    btnRemove->setMaximumWidth(75);
    btnRemove->setEnabled(false);
    btnRemove->hide();
    QPushButton* btnAdd = new QPushButton("+", this);
    btnAdd->setMaximumWidth(75);
    btnAdd->setEnabled(false);
    btnAdd->hide();

    commandLayout->addWidget(label, row, 0);
    commandLayout->addWidget(edit, row, 1);
    commandLayout->addWidget(btnRemove, row, 2);
    commandLayout->addWidget(btnAdd, row, 3);
}

void MainDialog::enableUi(bool enable)
{
    btnOpen->setEnabled(enable);
    btnSave->setEnabled(enable);
    btnClean->setEnabled(enable);
    btnRun->setEnabled(enable);

    for (int i = 0; i < commandLayout->rowCount(); i++) {
        for (int j = 1; j < 4; j++) {
            QLayoutItem* item = commandLayout->itemAtPosition(i, j);
            if (item) {
                item->widget()->setEnabled(enable);
            }
        }
    }

    resultEdit->setEnabled(enable);
}

QString MainDialog::createCommandJsonString()
{
    QJsonObject root;
    root.insert("dir", getCommandString(0));
    QString text;
    QJsonArray array;
    for (int i = 1; i < commandLayout->rowCount(); i++) {
        text = getCommandString(i);
        if (!text.isEmpty()) {
            array.append(getCommandString(i));
        }
    }
    root.insert("list", array);
    root.insert("variables", varMgr.getLocalFixVariables()->toJson());
    return QString(QJsonDocument(root).toJson());
}

QString MainDialog::getCommandString(int row)
{
    QString cmd;
    QLayoutItem* item = commandLayout->itemAtPosition(row, 1);
    if (item) {
        QLineEdit* edit = qobject_cast<QLineEdit*>(item->widget());
        if (edit) {
            cmd = edit->text();
        }
    }
    return cmd;
}

void MainDialog::setCommandString(int row, const QString& text)
{
    QLayoutItem* item = commandLayout->itemAtPosition(row, 1);
    if (!item) {
        addNewCommandEdit(row);
    }
    qobject_cast<QLineEdit*>(
        commandLayout->itemAtPosition(row, 1)->widget()
                )->setText(text);
}

bool MainDialog::checkItemJson(const QJsonObject &root)
{
    if (!root["dir"].isString()) {
        QMessageBox::critical(this, "错误", "dir字段必须是字符串");
        return false;
    }

    {
        if (!root["list"].isArray()) {
            QMessageBox::critical(this, "错误", "list field should be array");
            return false;
        }
        QJsonArray array = root["list"].toArray();
        QJsonArray::iterator it = array.begin();
        for (; it != array.end(); ++it) {
            if (!it->isString()) {
                QMessageBox::critical(this, "错误", "list数组中的item必须是字符串");
                return false;
            }
        }
    }

    {
        if (!root["variables"].isObject()) {
            QMessageBox::critical(this, "错误", "variables字段必须是json对象");
            return false;
        }
        QJsonObject obj = root["variables"].toObject();
        QJsonObject::const_iterator it = obj.begin();
        for (; it != obj.end(); ++it) {
            if (!it.value().isString()) {
                QMessageBox::critical(this, "错误", "variables对象中的value必须是字符串");
                return false;
            }
        }
    }
    return true;
}

void MainDialog::loadItem(const QString &json_val)
{
    QJsonObject root;
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(json_val.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "错误", "解析json文件失败");
        return;
    }
    if (!doc.isObject()) {
        QMessageBox::critical(this, "错误", "json文件格式错误：要求json文件必须是一个object");
        return;
    }
    root = doc.object();

    if (!checkItemJson(root)) {
        return;
    }
    setCommandString(0, root["dir"].toString());
    QJsonArray array = root["list"].toArray();
    QJsonArray::iterator it = array.begin();
    if (it != array.end()) {
        setCommandString(1, it->toString());
    }
    int commandCount = 2;
    for (++it; it != array.end(); ++it) {
        setCommandString(commandCount++, it->toString());
    }

    varMgr.getLocalFixVariables()->fromJson(root["variables"].toObject());
    {
        std::vector<std::pair<QString, QString>> vec;
        QJsonObject obj = root["variables"].toObject();
        QJsonObject::iterator it = obj.begin();
        for (; it != obj.end(); ++it) {
            vec.push_back(std::make_pair(it.key(), it.value().toString()));
        }
        placeHolderDlg->setVariableList(vec);
    }
}

int MainDialog::removeLastCommand()
{
    int pos = commandLayout->rowCount() - 1;
    if (pos <= 0) {
        return 0;
    }
    int removeCount = 0;
    for (; pos > 0; --pos) {
        QLayoutItem* item = commandLayout->itemAtPosition(pos, 0);
        if (item) {
            for (int i = 0; i < 4; i++) {
                QLayoutItem* item = commandLayout->itemAtPosition(pos, i);
                QPushButton* btn = (QPushButton*)item->widget();
                btn->setParent(nullptr);
                delete btn;
            }
            removeCount = 1;
            break;
        }
    }
    return removeCount;
}

void MainDialog::openFromJsonFile(const QString& jsonFileName)
{
    QFile file(jsonFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::invalid_argument("打开文件失败");
    }

    int removeCount = removeLastCommand();
    while (removeCount) {
        removeCount = removeLastCommand();
    }

    QTextStream stream(&file);
    QString json_val = stream.readAll();
    file.close();

    loadItem(json_val);
}

void MainDialog::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void MainDialog::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(!mimeData->hasText())
    {
        return;
    }
    QString fileName = mimeData->text();

    QString fileNamePrefix = "file:///";
    if (!fileName.startsWith(fileNamePrefix)) {
        return;
    }
    fileName = fileName.mid(fileNamePrefix.length());
    try {
        openFromJsonFile(fileName);
    } catch (const QException& err) {
        QMessageBox::critical(this, "错误", QString("打开失败: %1").arg(err.what()));
    }  catch (const std::exception& err) {
        QMessageBox::critical(this, "错误", QString("打开失败: %1").arg(err.what()));
    }
}

void MainDialog::onButtonAddCommandClicked()
{
    addNewCommandEdit(commandLayout->rowCount());
}

void MainDialog::onButtonRemoveCommandClicked()
{
    removeLastCommand();
}

void MainDialog::onButtonOpenClicked()
{
    try {
        QString fileName = QFileDialog::getOpenFileName(this, "打开", QString(), "*.json");
        if (fileName.isNull()) {
            return;
        }
        openFromJsonFile(fileName);
    }  catch (const QException& err) {
        QMessageBox::critical(this, "错误", QString("打开失败: %1").arg(err.what()));
    }  catch (const std::exception& err) {
        QMessageBox::critical(this, "错误", QString("打开失败: %1").arg(err.what()));
    }
}

void MainDialog::onButtonSaveClicked()
{
    QString file_name = QFileDialog::getSaveFileName(this, "保存", QString(), "*.json");
    if (file_name.isNull()) {
        return;
    }
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly| QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "保存失败");
        return;
    }

    QString json_val = createCommandJsonString();
    QTextStream stream(&file);
    stream << json_val;
    stream.flush();
    file.close();
}

void MainDialog::onButtonPlaceHolderClicked()
{
    if (placeHolderDlg->doModal() == QDialog::Accepted) {
        varMgr.setLocalFixVariables(*placeHolderDlg->getVariableList());
    }
}

void MainDialog::onButtonCleanClicked()
{
    /*while (commandLayout->rowCount() > 2) {
        for (int i = 0; i < 4; i++) {
            QLayoutItem* item = commandLayout->itemAtPosition(
                commandLayout->rowCount() - 1, i
            );
            commandLayout->removeItem(item);
        }
    }
    for (int i = 0; i < commandLayout->rowCount(); i++) {
        QLayoutItem* item = commandLayout->itemAtPosition(i, 1);
        QLineEdit* edit = qobject_cast<QLineEdit*>(item->widget());
        edit->clear();
    }*/

    resultEdit->clear();
}

void MainDialog::onButtonRunClicked()
{
    enableUi(false);
    resultEdit->setText("");

    runner.clean();
    QString dir = getCommandString(0);
    dir = varMgr.replace(dir);
    for (int i = 1; i < commandLayout->rowCount(); i++) {
        QString text = getCommandString(i);
        if (!text.isEmpty()) {
            QString cmd = varMgr.replace(text);
            runner.appendCommand(dir, cmd);
        }
    }
    runner.run();

    enableUi(true);
    resultEdit->setText(runner.getStdoutString());
}

