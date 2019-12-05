#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);

    QThread _thread;

    bool event(QEvent *ev)
    {
        qDebug() << "event:"<<QThread::currentThread();
        return QObject::event(ev);
    }

signals:

public slots:
    void doWork()
    {
        // do ......
        qDebug() << QThread::currentThread();
    }
};

#endif // WORKER_H
