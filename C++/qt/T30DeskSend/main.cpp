#include "sender.h"
#include <QApplication>
#include <QTimer>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Sender* sender = new Sender;

    QTimer* timer = new QTimer;
    app.connect(timer, SIGNAL(timeout()), sender, SLOT(slotSendOneFrame()));
    timer->setInterval(200);
    timer->start();


    return app.exec();
}
