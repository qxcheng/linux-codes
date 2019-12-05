#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);

    /* 窗口事件处理（所有事件） */
    bool event(QEvent *);

	//鼠标相关事件
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);   
 // void mouseDoubleClickEvent(QMouseEvent *); //不要用 

	//键盘相关事件
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

	
    void closeEvent(QCloseEvent *); //窗口关闭事件
 // void showEvent(QShowEvent *);
 // void hideEvent(QHideEvent *);
    void paintEvent(QPaintEvent *); //绘图

    QPushButton* button;
    QLineEdit* edit;

signals:

public slots:
    void slotButtonClicked();  //槽函数

};

#endif 