#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString str = (tr("Open"));
    QMessageBox::warning(this, "Error", this->ui->lineEdit->text());
}

void MainWindow::on_button1_clicked()
{

    QMessageBox::warning(this, "Error", this->ui->lineEdit->text());
}

void MainWindow::on_action_Open_triggered()
{
    QMessageBox::warning(this , "aaa", "bbb");
}
