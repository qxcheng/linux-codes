
#include <QCoreApplication>
#include "mythread.h"
#include "worker.h"
#include <QTimer>
int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

#if 0
    MyThread thread;
    thread.start();

    thread.foo();
#endif

    qDebug() << "main thread is"<<QThread::currentThread();
    Worker* worker = new Worker();
    QTimer* timer = new QTimer;

    QObject::connect(timer, SIGNAL(timeout()), worker, SLOT(doWork()));
    timer->setInterval(1000);
    timer->start();

    return app.exec();
}
