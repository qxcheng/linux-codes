#ifndef WIDGET03_H
#define WIDGET03_H

#include <QObject>
#include <QSqlTableModel>
#include <QTableView> // show table

class Widget03 : public QWidget
{
    Q_OBJECT
public:
    explicit Widget03(QWidget *parent = 0);

    QSqlTableModel* _model;
    QTableView* _view;

signals:

public slots:
    void slotSubmitClicked();
    void slotDelClicked();
    void slotAddClicked();
};

#endif // WIDGET03_H
