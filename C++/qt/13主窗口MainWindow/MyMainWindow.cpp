#include "MyMainWindow.h"
#include <QApplication>

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>
#include <QIcon>

MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    /* 加菜单 */
    QMenuBar* pMenuBar = menuBar();
    QMenu* menu = pMenuBar->addMenu("&File");
    _menu = menu;
    QAction* openAction = menu->addAction("&Open", this, SLOT(slotOpen()), QKeySequence::Open);
    QAction* saveAction = menu->addAction("&Save", this, SLOT(slotOpen()), QKeySequence::Save);

    menu->addSeparator();
	
    QAction* closeAction = menu->addAction("&Exit", this, SLOT(close()), QKeySequence::Close);
    closeAction->setToolTip("close window");

    /* 工具栏 */
    QToolBar* toolBar = this->addToolBar("MyToolBar");
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(closeAction);

    /* 状态栏 */
    QStatusBar* pStatusBar = this->statusBar();
    pStatusBar->addWidget(_label = new QLabel("OK"));
    _label->setText("<font color=red>Processing....</font>");

    /* 别的控件占用了之后，剩下的区域都是CentralWidget */
    _view = new MyView;
    this->setCentralWidget(_view);

    /* 系统托盘小图标 */
    _icon = new QSystemTrayIcon;
    _icon->setIcon(QIcon("../bing.ico"));
    _icon->setToolTip("This is tray icon test");
    _icon->show();
    _icon->setContextMenu(_menu);  //鼠标聚焦的菜单

    connect(_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(slotActivated(QSystemTrayIcon::ActivationReason)));

    this->installEventFilter(this);
}

void MyMainWindow::slotActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        if(this->isHidden()) this->show();
        else this->hide();
    }
}

bool MyMainWindow::eventFilter(QObject *o, QEvent *e)
{
    if(o == (QObject*)this && e->type() == QEvent::Close)
    {
        return true;
    }

    return QMainWindow::eventFilter(o, e);
}

bool MyMainWindow::event(QEvent *ev)
{
    qDebug() << ev;
    if(ev->type() == QEvent::Close)
    {
        return false;
    }

    return QMainWindow::event(ev);
}

//鼠标右键菜单
void MyMainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::RightButton)
        _menu->exec(QCursor::pos());
}

void MyMainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(QPoint(0, 0), QPixmap("../aaa.png"));
}

void MyMainWindow::slotOpen()
{
    QString strFile = QFileDialog::getOpenFileName();
    qDebug() << "Open file is:" << strFile;
}


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MyMainWindow w;
    w.show();

    return app.exec();
}
