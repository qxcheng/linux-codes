
#include <QApplication>
#include "mywidget.h"
#include "recver.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MyWidget w;
    w.show();

    Recver* recver = new Recver;

    app.connect(recver, SIGNAL(sigFrameArrive(QByteArray)),
                &w, SLOT(slotFrameArrive(QByteArray)));



    return app.exec();
}
