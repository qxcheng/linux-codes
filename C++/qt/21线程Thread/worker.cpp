#include "worker.h"

Worker::Worker(QObject *parent) :
    QObject(parent)
{
   // this->moveToThread(&_thread);
    _thread.start();
    connect(&_thread, SIGNAL(finished()), this, SLOT(deleteLater()));
}
