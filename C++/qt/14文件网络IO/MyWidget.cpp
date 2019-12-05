#include "MyWidget.h"
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QBuffer>
#include <QLabel>
#include <QTextStream>
#include <QDataStream>
#include "TcpServer.h"
#include "TcpClient.h"
#include "udp1.h"
#include "udp2.h"
/*
    QFile
    QBuffer // memory file
    QTcpSocket
    QUdpSocket

    QDataStream  // io method
    QTextStream
*/

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
#if 0
    QFile file("../MyTest.txt");
    file.open(QIODevice::ReadWrite);

    file.write(QByteArray("abc"));
    // file.seek
   // file.write()

    file.close();
#endif

#if 0
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);

    buffer.write("abc");
    buffer.write("aabbcc");

    buffer.close();

    qDebug() << buffer.buffer();
#endif

#if 0
    // send side
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    QPixmap pixmap("../aaa.png");
    pixmap.save(&buffer, "PNG");
    buffer.close();

    qDebug() << buffer.buffer().size();

    // recv side
    QPixmap pixmap2;
    pixmap2.loadFromData(buffer.buffer(), "PNG");

    QLabel* label = new QLabel(this);
    label->setPixmap(pixmap2);
#endif


#if 0
    QByteArray ba("薛国良");
    QString str("薛国良");

    qDebug() << ba.size() << str.size();
#endif

#if 0
    QFile file("../teststream.txt");
    file.open(QIODevice::ReadWrite);

    QTextStream textStream(&file);

    textStream << 1 << "abc" << 1.2 << QString("abc");

    file.close();
#endif
#if 0
    QFile file("../datastream.txt");
    file.open(QIODevice::ReadWrite);

    QDataStream dataStream(&file);

    dataStream << 1 << "abc" << 1.2 << QString("abc") << QPoint(1, 1);

    file.close();
#endif

#if 0
    QFile file("../datastream.txt");
    file.open(QIODevice::ReadWrite);

    QDataStream dataStream(&file);

    int i;
    char* buf;
    double d;
    QString str;
    QPoint pt;

    dataStream >> i >> buf >> d >> str >> pt;

    delete[] buf;

    qDebug() << i << buf << d << str << pt;
    file.close();

#endif

#if 0
    // virtual memory
    QFile file("../map.txt");

    file.open(QIODevice::ReadWrite);
    file.seek(65535);
    file.write("1");
    file.close();
    file.open(QIODevice::ReadWrite);

    uchar* ptr = file.map(0, 64*1024);

    // write file
    *ptr = 'a';
    ptr[1] = 'b';

    // read file
    uchar ch = *ptr;

    file.unmap(ptr);
    file.close();
#endif
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

#if 0
  //  MyWidget w;
  //  w.show();
    TcpServer s; s.show();
    TcpClient c; c.show();
    s.setWindowTitle("server");
    c.setWindowTitle("client");
#endif
    Udp1 udp1; udp1.show();
    Udp2 udp2; udp2.show();
    udp1.setWindowTitle("udp1");
    udp2.setWindowTitle("udp2");

    return app.exec();
}


