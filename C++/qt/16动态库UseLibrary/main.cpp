
#include <QCoreApplication>

#include "../T14Library/t14library.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    T14Library d;
    d.Encrypt();

    app.exec();
}
