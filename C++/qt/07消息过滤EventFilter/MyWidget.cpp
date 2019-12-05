#include "MyWidget.h"
#include "MyApplication.h"
#include <QPushButton>
#include <QEvent>
#include <QDebug>
#include <QApplication>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
    QPushButton* button;

    button = new QPushButton("This button", this);
    connect(button, SIGNAL(clicked()), this, SLOT(close()));
    _button = button;

    /* button给自己安装了一个消息过滤器，那么经过button的消息，
	都先要调用它的过滤器的eventFilter函数 */
    button->installEventFilter(this);
}

bool MyWidget::eventFilter(QObject *o, QEvent *e)
{
#if 1
    if(o == (QObject*)_button &&(
                e->type() == QEvent::MouseButtonRelease ||
                e->type() == QEvent::MouseButtonDblClick ||
                e->type() == QEvent::MouseButtonPress))
    {

        return true;
    }
#endif

    return QWidget::eventFilter(o, e);
}

bool MyWidget::event(QEvent *e)
{
    if(e->type() == QEvent::User)
    {
        qDebug() << "User event is comming";
    }
    return QWidget::event(e);
}


int main(int argc, char* argv[])
{
    MyApplication app(argc, argv);

    MyWidget w;
    w.show();

    //自定义消息，发送一个Event给MyWidget
    qDebug() << "begin send";
    app.postEvent(&w, new QEvent(QEvent::User)); //延迟发送
    qDebug() << "end send";
  //app.sendEvent(&w, new QEvent(QEvent::User)); //直接发送

    return app.exec();
}
