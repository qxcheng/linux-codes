
#include <QCoreApplication>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QFile>
#include <QDebug>
int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QFile file("d:\\qt-record.raw");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << "open file error";
        exit(0);
    }

    /*
     *  qDebug() <<   format.sampleRate();
      qDebug() <<  format.sampleSize();
     qDebug() <<   format.sampleType();
      qDebug() <<  format.codec();
      qDebug() <<  format.byteOrder();
     qDebug() <<   format.channelCount();

8000
8
SignedInt
"audio/pcm"
LittleEndian
1
*/
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setSampleSize(8);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(1);

    QAudioOutput* output = new QAudioOutput(format);

    output->start(&file);

    return app.exec();
}
