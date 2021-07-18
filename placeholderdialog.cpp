#include "placeholderdialog.h"
#include "ui_placeholderdialog.h"

#include <QPushButton>
#include <QMessageBox>

namespace {

QString addNewVariableStr = "add a new variable";

}

PlaceHolderDialog::PlaceHolderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaceHolderDialog)
{
    ui->setupUi(this);
    initUi();
}

PlaceHolderDialog::~PlaceHolderDialog()
{
    delete ui;
}

int PlaceHolderDialog::doModal()
{
    resetTableView();
    return exec();
}

const std::vector<std::pair<QString, QString> > *PlaceHolderDialog::getVariableList() const
{
    return &variableList;
}

void PlaceHolderDialog::setVariableList(const std::vector<std::pair<QString, QString> > &vars)
{
    variableList = vars;
}

void PlaceHolderDialog::initUi()
{
    mainLayout = new QVBoxLayout(this);
    model = new QStandardItemModel(this);
    tableView = new QTableView(this);
    tableView->setModel(model);
    btnBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);

    connect(model, &QStandardItemModel::itemChanged, this, &PlaceHolderDialog::onTableViewItemChanged);
    connect(btnBox->button(QDialogButtonBox::Save), &QPushButton::clicked,
            this, &PlaceHolderDialog::onAccept);
    connect(btnBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
            this, &PlaceHolderDialog::reject);

    mainLayout->addWidget(tableView);
    mainLayout->addWidget(btnBox);
    this->setLayout(mainLayout);

    btnBox->setCenterButtons(false);
    btnBox->button(QDialogButtonBox::Save)->setText("Save");
    btnBox->button(QDialogButtonBox::Cancel)->setText("Cancel");
}

void PlaceHolderDialog::resetTableView()
{
    model->clear();
    model->setColumnCount(2);
    model->setHeaderData(0, Qt::Horizontal, "Variable");
    model->setHeaderData(1, Qt::Horizontal, "Value");
    for (const auto& pair : variableList) {
        model->appendRow(QList<QStandardItem*> {
                             new QStandardItem(pair.first),
                             new QStandardItem(pair.second)
                         });
        tableView->resizeColumnsToContents();
    }
    model->appendRow(QList<QStandardItem*> {
                         new QStandardItem(addNewVariableStr),
                         new QStandardItem("")
                     });
    tableView->resizeColumnsToContents();
}

void PlaceHolderDialog::onTableViewItemChanged(QStandardItem *item)
{
    // 格式化：若变量名不以${开头，或不以}结尾，则格式化为标准形式
    QString dataStr = item->data(Qt::DisplayRole).toString();
    QString currentDataStr = dataStr.trimmed();
    if (item->column() == 0) {
        if (!currentDataStr.startsWith("${")) {
            currentDataStr = "${" + currentDataStr;
        }
        if (!currentDataStr.endsWith("}")) {
            currentDataStr.append("}");
        }
    }
    if (dataStr != currentDataStr) {
        model->setData(model->index(item->row(), item->column()), currentDataStr);
        tableView->resizeColumnsToContents();
    }
    // 增加新行
    int currentRow = item->row();
    if (currentRow != (model->rowCount() - 1)) {
        return;
    }
    QStandardItem* col1 = model->item(currentRow, 0);
    QString colStr = col1->data(Qt::DisplayRole).toString();
    if (colStr.isEmpty() || colStr == addNewVariableStr) {
        return;
    }
    model->appendRow(QList<QStandardItem*> {
                         new QStandardItem(addNewVariableStr),
                         new QStandardItem("")
                     });
    tableView->resizeColumnsToContents();
}

void PlaceHolderDialog::onAccept()
{
    variableList.clear();
    int rowCount = model->rowCount();
    for (int i = 0; i < rowCount; i++) {
        QString key = model->data(model->index(i, 0)).toString();
        QString val = model->data(model->index(i, 1)).toString();
        if (key.isEmpty() || key == addNewVariableStr) {
            continue;
        }
        variableList.push_back(std::make_pair(key, val));
    }
    accept();
}
