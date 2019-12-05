


#include <QApplication>
#include <QLabel>
#include <QPixmap>

int main(int argc, char** argv)
{
    // 19K
    QApplication app(argc, argv);

    /* init resource */
    Q_INIT_RESOURCE(main);

    QLabel label;
    label.setPixmap(QPixmap(":/aaa.png"));

    label.show();
    return app.exec();
}
