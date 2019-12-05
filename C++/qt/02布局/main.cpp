#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>      //只显示文本
#include <QVBoxLayout> //垂直方向
#include <QHBoxLayout> //水平方向
#include <QGridLayout> //格子

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QWidget w;

/*	绝对位置布局
	QPushButton button;
    button.setText("Button");
    button.setParent(&w);
    button.show();
    button.setGeometry(30, 30, 100, 30); //以左上角为原点设置位置
*/

#if 0
    QPushButton button;
    button.setText("Button");

    QLineEdit edit;

    QHBoxLayout layout;
    layout.addStretch(1);   //水平增加比重
    layout.addWidget(&button, 1);
    layout.addSpacing(100); //增加间距
    layout.addWidget(&edit, 1);
    layout.addStretch(1);
#endif

#if 0
    QGridLayout layout;

    layout.setColumnStretch(3, 1); //在第3列占比为1的弹簧
    layout.setRowStretch(4, 1);    //在第四行占比为1的弹簧
    layout.setColumnStretch(0, 1);
    layout.setRowStretch(0, 1);

    layout.addWidget(new QPushButton("Button"), 1, 1);
    layout.addWidget(new QLineEdit(), 1, 2);
    layout.addWidget(new QPushButton("1, 0"), 2, 1);
    layout.addWidget(new QLineEdit(), 2, 2);

    //（3，1）处行（向下）拓展为1，列拓展为2
    layout.addWidget(new QPushButton("aaa"), 3, 1, 1, 2);
#endif

#if 1
    QGridLayout layout;
    QLineEdit* password;
    layout.setColumnStretch(3, 1);
    layout.setRowStretch(4, 1);
    layout.setColumnStretch(0, 1);
    layout.setRowStretch(0, 1);

    layout.addWidget(new QLabel("Username:"), 1, 1);
    layout.addWidget(new QLineEdit(), 1, 2);
    layout.addWidget(new QLabel("Password:"), 2, 1);
    layout.addWidget(password = new QLineEdit(), 2, 2);
    password->setEchoMode(QLineEdit::Password);

    QHBoxLayout* hBox;
    layout.addLayout(hBox = new QHBoxLayout, 3, 2);
    hBox->addStretch(1);
    hBox->addWidget(new QPushButton("Login"));
#endif

    w.show();  /*显示窗口*/
    w.setLayout(&layout);

    w.setWindowTitle("Hello World");
    return app.exec();
}
