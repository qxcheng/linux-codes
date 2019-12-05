#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "MyView.h"
#include <QSystemTrayIcon>

class MyMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MyMainWindow(QWidget *parent = 0);

    QLabel* _label;
    MyView* _view;

    QSystemTrayIcon* _icon;

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

    QMenu* _menu;
    bool event(QEvent *event);
    bool eventFilter(QObject *, QEvent *);

signals:

public slots:
    void slotOpen();
    void slotActivated(QSystemTrayIcon::ActivationReason);


};

#endif // MYMAINWINDOW_H
