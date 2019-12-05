

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <stdio.h>
int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    app.setApplicationVersion("1.0.0.0");
    app.setApplicationName("xxxx tools");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption p(QStringList() << "p" << "package", "set package name of Android", "com.itcast.hello");
    parser.addOption(p);
    QCommandLineOption l(QStringList() << "l" << "language", "set code language c++ or lua", "c++");
    parser.addOption(l);

//    parser.parse(app.arguments());
    parser.process(app);

    QString strValue = parser.value("p"); /* package */
  //  qCritical() <<"p is"<< strValue;
    printf("p is %s\n", strValue.toUtf8().data());
    strValue = parser.value("l"); /* language */
    printf("l is %s\n", strValue.toUtf8().data());


   qDebug() << parser. positionalArguments();

    return 0;
//    return app.exec();
   // return app.exec();
}
