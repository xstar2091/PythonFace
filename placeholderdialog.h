#ifndef PLACEHOLDERDIALOG_H
#define PLACEHOLDERDIALOG_H

#include <QDialog>

#include <vector>
#include <utility>

#include <QDialogButtonBox>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>

namespace Ui {
class PlaceHolderDialog;
}

// https://blog.csdn.net/qq_42123832/article/details/105889547
class PlaceHolderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlaceHolderDialog(QWidget *parent = nullptr);
    ~PlaceHolderDialog();

    int doModal();
    const std::vector<std::pair<QString, QString>>* getVariableList() const;
    void setVariableList(const std::vector<std::pair<QString, QString>>& vars);

private:
    void initUi();
    void resetTableView();
    void onTableViewItemChanged(QStandardItem *item);
    void onAccept();

private:
    Ui::PlaceHolderDialog *ui;
    QVBoxLayout* mainLayout;
    QTableView* tableView;
    QStandardItemModel* model;
    QDialogButtonBox* btnBox;

    std::vector<std::pair<QString, QString>> variableList;
};

#endif // PLACEHOLDERDIALOG_H
