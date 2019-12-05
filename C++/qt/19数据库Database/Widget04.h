#ifndef WIDGET04_H
#define WIDGET04_H

#include <QObject>
#include <QSqlTableModel>
#include <QTableView> // show table
#include <QItemDelegate>
#include <QComboBox>

class TUserDelegate : public QItemDelegate
{
    QWidget *createEditor(QWidget * parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        if(index.column() == 0)
            return NULL;
        if(index.column() == 3)
        {
            QComboBox*  combo = new QComboBox(parent);
            combo->addItem("男");
            combo->addItem("女");
            return combo;
        }

        return QItemDelegate::createEditor(parent, option, index);
    }
};
#if 0
class ReadOnlyDelegate : public QItemDelegate
{
    QWidget *createEditor(QWidget *,
                          const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        return NULL;
    }
};
class GenderDelegate : public QItemDelegate
{
public:
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &,
                          const QModelIndex &) const
    {
        QComboBox*  combo = new QComboBox(parent);
        combo->addItem("男");
        combo->addItem("女");
        return combo;
    }
};
#endif

class MyTableModel : public QSqlTableModel
{
public:
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const
    {
     //   if(role == Qt::DisplayRole)
     //       return QSqlTableModel::data(idx, role);
        if(idx.column() != 3)
            return QSqlTableModel::data(idx, role);

        QVariant var = QSqlTableModel::data(idx, role);
        if(var == 0)
        {
            return "女";
        }

        return "男";
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole)
    {
        if(index.column() != 3)
            return QSqlTableModel::setData(index, value, role);

        if(value == "男")
            return QSqlTableModel::setData(index, 1, role);
        return QSqlTableModel::setData(index, 0, role);
    }

};

class Widget04 : public QWidget
{
    Q_OBJECT
public:
    explicit Widget04(QWidget *parent = 0);

    MyTableModel* _model;
    QTableView* _view;

signals:

public slots:
    void slotSubmitClicked();
    void slotDelClicked();
    void slotAddClicked();
};

#endif // WIDGET04_H
