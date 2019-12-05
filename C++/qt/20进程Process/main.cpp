#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    QProcess process;
   // process.start("/home/xuegl/T0718/build-T18Database-Desktop-Debug/T18Database");
   process.start("ssh root@42.121.13.248");
 //  process.start("ssh", QStringList() << "root@42.121.13.248" << "aa" << "bbb");
//   process.write("1\n", 2);
   process.waitForFinished();

 //   process.waitForFinished();
    qDebug() << process.readAll();
 //   qDebug() << process.exitCode();

   return app.exec();
}
