#include "MyWidget.h"
#include <QApplication>   /* 应用程序抽象类 */
#include <QLabel>        // 静态显示文本或图片
#include <QPushButton>   // 按钮
#include <QLineEdit>     // 单行输入框
#include <QCompleter>    //输入框补全提示
#include <QComboBox>     // 下拉选择框
#include <QCheckBox>     // 方形复选框
#include <QRadioButton>  // 圆形复选框
#include <QTextEdit>     // 多行、富文本输入框
#include <QTextBrowser>  // 只读的输入框
#include <QGroupBox>     // 群组用于分类管理
#include <QSlider>       // 滑块，模拟方式显示数值
#include <QSpinBox>      // 数字框
#include <QDateEdit>
#include <QTimeEdit>
#include <QDateTimeEdit>
#include <QTabWidget>    // 不介绍,不算控件
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QPixmap>
#include <QLCDNumber>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
	//垂直方向布局
    QVBoxLayout* lay = new QVBoxLayout(this);
    
	//静态显示文本或图片
	QLabel* label;
    lay->addWidget(label = new QLabel("<a href=www.baidu.com>baidu</a>"));
    label->setPixmap(QPixmap("../aaa.png"));
    connect(label, &QLabel::linkActivated, [](QString str){
        qDebug() << str;
    });
	
	//按钮
	QPushButton* button;
    lay->addWidget(button = new QPushButton("Button"));
    button->setStyleSheet("QPushButton {font:bold 16px; color:red; padding:5px}");
	QObject::connect(button, SIGNAL(clicked()), this, SLOT(close()));
	
	// 单行输入框  ？？？
	QLineEdit *edit;
    lay->addWidget(edit = new QLineEdit());
  //edit->setEchoMode(QLineEdit::Password);  /* 输入密码 */
  //edit->text();  //获取输入框文字
  //edit->setPlaceholderText("Please input text:"); //输入框提示
    QCompleter completer(QStringList() << "aab" << "123" ); //设置补全内容
    completer.setFilterMode(Qt::MatchContains); //设置补全提示方式
    edit->setCompleter(&completer);  //将二者关联
	
	//圆形复选框
	QRadioButton* radio;
    lay->addWidget(radio = new QRadioButton("Radio"));
    radio->setStyleSheet("QRadioButton {font:bold 16px; color:red; padding:5px}");
    connect(radio, &QRadioButton::clicked, [](bool v){
        qDebug() << v;
    });

	//方形复选框
    lay->addWidget(new QCheckBox("check box"));

	//下拉选择框 
	QComboBox* combo;
    lay->addWidget(combo = new QComboBox());
    combo->addItem("Select item1");
    combo->addItem("Select item2");
    combo->setEditable(true);
    connect(combo, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotComboBoxIndexChanged(QString)));
    combo->setCompleter(new QCompleter(combo->model()));
	
	//富文本输入框
	QTextEdit* edit;
    lay->addWidget(edit = new QTextEdit);
    edit->setText("<table border=1><tr><th>head1</th><th>head2</th></tr>"
                  "<tr><td>value1</td><td>value2</td></tr>"
                  "<tr><td>value3</td><td>value4</td></tr>"
                  "</table><br>"
                  "<img src=../aaa.png></img>");
    _edit = edit;
    connect(edit, &QTextEdit::textChanged, [&](){
     // QTextEdit* edit = (QTextEdit*)this->sender();
        qDebug() << _edit->toPlainText();
    });
    edit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    edit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	
	//群组管理框
	QGroupBox* group;
    lay->addWidget(group = new QGroupBox("Some items"));
    QHBoxLayout* hBoxLay;
    group->setLayout(hBoxLay = new QHBoxLayout);
    hBoxLay->addWidget(new QPushButton("aaaa"));
    hBoxLay->addWidget(new QPushButton("aaaa"));

	//滑块
    QSlider* slider;
    lay->addWidget(slider = new QSlider(Qt::Horizontal));
    slider->setMaximum(100);
    slider->setMinimum(0);
	//slider->setRange(0,100);
	//数字框
	QSpinBox* spinBox;
    lay->addWidget(spinBox = new QSpinBox);
    spinBox->setMaximum(100);
    spinBox->setMinimum(0);
	//二者结合使用
    connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
    connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));

	//日期框
    lay->addWidget(new QDateTimeEdit);

	//lcd屏显示
    QLCDNumber* lcd;
    lay->addWidget(lcd = new QLCDNumber(10));
    lcd->display(12345);
    lcd->setMode(QLCDNumber::Hex);
    lcd->setSegmentStyle(QLCDNumber::Outline);
}

void MyWidget::slotComboBoxIndexChanged(const QString &str)
{
    qDebug() << "slotComboBoxIndexChanged" << str;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MyWidget w;
	w.setWindowTitle("Hello World");
    w.show();

    return app.exec();
}
