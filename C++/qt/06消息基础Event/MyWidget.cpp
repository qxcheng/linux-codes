/* QApplication先得到->具体应该处理的窗口::event()->event()根据消息类型来调用具体的虚函数 */
/* 1）可以重载具体的虚函数，来实现对消息的响应
   2）可以重载event函数，用来处理或者截取消息 */
#include "MyWidget.h"
#include <QApplication>
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QPainter>

/* 构造函数 */
MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* lay = new QVBoxLayout(this);
  //QVBoxLayout*lay =new QVBoxLayout();
  //this->setLayout(lay);
  
    // 鼠标不需要按下，mouseMove就能得到调用
    this->setMouseTracking(true);
	
#if 0
    QPushButton* button2;
    lay->addWidget(button = new QPushButton("OK", this));
    lay->addWidget(button2 = new QPushButton("Button2"));

	//button被设为默认（蓝色），回车直接响应
	//上下选择暂时只能使用空格响应
    button->setDefault(true); 

    connect(button2, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
    connect(button, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
#endif
//  lay->addWidget(edit = new QLineEdit());
//  connect(edit, SIGNAL(returnPressed()), this, SLOT(slotButtonClicked()));
}

/* 槽函数实现按钮点击的响应 */
void MyWidget::slotButtonClicked()
{
    QPushButton* button = (QPushButton*)sender();
  //QLineEdit* edit = (QLineEdit*)sender();
    qDebug() << button->text();
}


/* 截取消息 */
bool MyWidget::event(QEvent *ev)
{
  //鼠标消息被截断
  //if(ev->type() == QEvent::MouseButtonPress)
  //    return true;
    ev->accept();

    return QWidget::event(ev);
}

/* 关闭事件 */
void MyWidget::closeEvent(QCloseEvent *)
{
    qDebug() << "closeEvent";
}

/* 绘图事件 */
void MyWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawLine(QPoint(0, 0), QPoint(100, 100));

}

/* 鼠标点击事件 */
void MyWidget::mousePressEvent(QMouseEvent *ev)
{
#if 0
    QPoint pt = ev->pos();
    qDebug() << pt;
    if(ev->button() == Qt::LeftButton)
    {

    }
    if(ev->modifiers() == Qt::ShiftModifier)
    {
        qDebug() << "shift press";
    }
#endif
    if(ev->button() == Qt::LeftButton)
    {
        if(ev->modifiers() == Qt::ControlModifier)
        {
            // handle with Control;
            return;
        }

        // handle2 without control;
    }
    else
    {

    }
}

/* 鼠标释放事件 */
void MyWidget::mouseReleaseEvent(QMouseEvent *)
{}

/* 鼠标移动事件 */
void MyWidget::mouseMoveEvent(QMouseEvent *)
{
    static int i=0;
    qDebug() << "mouse move"<< i++;
}

/* 键盘点击事件 */
void MyWidget::keyPressEvent(QKeyEvent *ev)
{
    ev->modifiers();
    int key = ev->key();
    qDebug() << key;
    char a = key;
    qDebug() << (char)a;
}

/* 键盘释放事件 */
void MyWidget::keyReleaseEvent(QKeyEvent *)
{}

/* 主函数 */
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MyWidget w;
    w.show();

    return app.exec();
}
