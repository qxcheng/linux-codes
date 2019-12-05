#include "MyDialog.h"
#include <QPushButton>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

#include <QColorDialog>  //颜色选择
#include <QFontDialog>   //字体选择
#include <QMessageBox>   //消息提示
#include <QPainter>    

MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent)
{
    QPushButton* button = new QPushButton("Click me", this);
    connect(button, SIGNAL(clicked()), this, SLOT(slotButtonClick()));
}

void MyDialog::slotButtonClick()
{
#if 0
    QDialog* dlg=new QDialog;
    int ret;
    QPushButton* button = new QPushButton(dlg);
    connect(button, SIGNAL(clicked()), dlg, SLOT(reject()));

    ret = dlg->exec();  // 在模块对话框中，exec有自己的消息循环，并且把app的消息循环接管了
    // 如果Dialog是通过exec来显示，那么可以通过accept或者reject来关闭窗口
    // 如果Dialog是通过show来显示，那么可以通过close来关闭窗口，这个和QWidget一样的

    // 有许多特殊的dailog：文件选择，MessageBox，颜色选择，字体选择，打印预览，打印

    if(ret == QDialog::Accepted)
    {
        qDebug() << "accepted";
    }
    if(ret == QDialog::Rejected)
    {
        qDebug() << "rejected";
    }
#endif

#if 0
	//保存文件选择
    QString strFilename = QFileDialog::getSaveFileName(NULL,
                                                       "Select file for save",
                                                       _strDir,
                                                       "pic file (*.png *.jpg)");
#endif
#if 0
	//打开文件选择
    QString strFilename = QFileDialog::getOpenFileName(NULL,
                                                       "Select file for open",
                                                       _strDir,
                                                       "pic file (*.png *.jpg)");
#endif
#if 0
	//打开目录
    QString strFilename = QFileDialog::getExistingDirectory();

    if(strFilename.isEmpty())
    {
        qDebug() << "select none";
        return;
    }
    qDebug() << strFilename;
	
    QFileInfo fileInfo(strFilename);
    _strDir = fileInfo.filePath();   //保存上次打开目录
#endif
    // do something for io ...

#if 0
	//颜色选择
    QColorDialog color;
    color.exec();
    QColor c = color.selectedColor();
#endif

#if 0
	//字体选择
    QFontDialog fontDialog;
    fontDialog.exec();
    QFont font = fontDialog.selectedFont();
#endif
	//消息提示
    int ret = QMessageBox::question(this, "????", "realy do .......",
                                    QMessageBox::Yes| QMessageBox::No|
                                    QMessageBox::YesAll| QMessageBox::NoAll);
    if(ret == QMessageBox::Yes)
    {
        qDebug() << "user select yes";
    }
    if(ret == QMessageBox::No)
    {
        qDebug() << "user select no";
    }

}
void MyDialog::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawLine(QLine(0, 0, 200, 200));
}


#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MyDialog dlg;
    dlg.show();

    return app.exec();
}
