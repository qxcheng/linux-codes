#include "Widget04.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlRecord>
Widget04::Widget04(QWidget *parent) :
    QWidget(parent)
{
    _model = new MyTableModel;
    _model->setTable("tuser");
    _model->select();
   _model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    _model->setHeaderData(0, Qt::Horizontal, "编号");
    _model->setHeaderData(1, Qt::Horizontal, "用户名");
    _model->setHeaderData(2, Qt::Horizontal, "密码");
    _model->setHeaderData(3, Qt::Horizontal, "性别");

    _view = new QTableView;
    _view->setModel(_model);
//    _view->hideColumn(2);

  //  _view->setItemDelegateForColumn(3, new GenderDelegate);
   // _view->setItemDelegateForColumn(0, new ReadOnlyDelegate);
    _view->setItemDelegate(new TUserDelegate);
    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->addWidget(_view);

    QHBoxLayout* hBox = new QHBoxLayout;
    lay->addLayout(hBox);
    hBox->addStretch();

    QPushButton* del = new QPushButton("del");
    connect(del, SIGNAL(clicked()), this, SLOT(slotDelClicked()));
    hBox->addWidget(del);

    QPushButton* submit = new QPushButton("submit");
    connect(submit, SIGNAL(clicked()), this, SLOT(slotSubmitClicked()));
    hBox->addWidget(submit);

    QPushButton* add = new QPushButton("add");
    connect(add, SIGNAL(clicked()), this, SLOT(slotAddClicked()));
    hBox->addWidget(add);
}
void Widget04::slotAddClicked()
{

    QSqlRecord record = _model->record();
    _model->insertRecord(-1, record);
}
void Widget04::slotDelClicked()
{
    // 通过_view去获取被选中的部分的数据model
    QItemSelectionModel * selectModel = _view->selectionModel();
// 通过选中的数据结构，获取这些格子的ModelIndex
    QModelIndexList selectList =  selectModel->selectedIndexes();
    QList<int> delRow;

    // 遍历这些格子，获取格子所在行，因为可能存在相同的行，所以要去重
    for(int i=0; i<selectList.size(); ++i)
    {
        QModelIndex index = selectList.at(i);
      //  _model->removeRow(index.row());
        delRow << index.row();
    }

    while(delRow.size() > 0)
    {
        int row = delRow.at(0);
        delRow.removeAll(row);
        _model->removeRow(row);
    }

    _model->submitAll();

}


void Widget04::slotSubmitClicked()
{
    if(!_model->submitAll())
    {
        QMessageBox::critical(this, "Error", QSqlDatabase().lastError().text());
    }
}
