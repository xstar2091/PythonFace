#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QJsonObject>

#include "placeholderdialog.h"

#include "commandrunner.h"
#include "variable/variablemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainDialog; }
QT_END_NAMESPACE

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

private:
    void initUi();
    void initCommandDirUi();
    void addNewCommandEdit(int row);
    void enableUi(bool enable);
    QString createCommandJsonString();
    QString getCommandString(int row);
    void setCommandString(int row, const QString& text);
    bool checkItemJson(const QJsonObject& root);
    void loadItem(const QString& json_val);
    int removeLastCommand();

    void openFromJsonFile(const QString& jsonFileName);

private slots:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void onButtonAddCommandClicked();
    void onButtonRemoveCommandClicked();
    void onButtonOpenClicked();
    void onButtonSaveClicked();
    void onButtonPlaceHolderClicked();
    void onButtonCleanClicked();
    void onButtonRunClicked();

private:
    Ui::MainDialog *ui;
    QVBoxLayout* mainVBoxLayout;
    QHBoxLayout* editHBoxLayout;// 编辑菜单栏布局
    QPushButton* btnOpen;
    QPushButton* btnSave;
    QPushButton* btnPlaceholder;
    QPushButton* btnClean;
    QPushButton* btnRun;

    QGridLayout* commandLayout;
    QTextEdit* resultEdit;

    PlaceHolderDialog* placeHolderDlg;

    CommandRunner runner;
    VariableManager varMgr;
};
#endif // MAINDIALOG_H
