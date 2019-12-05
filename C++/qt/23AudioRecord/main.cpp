

#include <QCoreApplication>
#include <QAudioInput>
#include <QAudioFormat>
#include <QFile>
#include <QTimer>
#include <QAudioDeviceInfo>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    // record audio
    QString audioFileName = "d:\\qt-record.raw";
    QFile audioFile(audioFileName);
    audioFile.open(QFile::ReadWrite);

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("codec/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);

    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();
    if(!device.isFormatSupported(format))
    {
        format = device.nearestFormat(format);

      qDebug() <<   format.sampleRate();
      qDebug() <<  format.sampleSize();
     qDebug() <<   format.sampleType();
      qDebug() <<  format.codec();
      qDebug() <<  format.byteOrder();
     qDebug() <<   format.channelCount();

    }

    QAudioInput* audio = new QAudioInput(format);

   // audio->start(&audioFile);

    QIODevice* audioData = audio->start();
    audio->setNotifyInterval(200);
    app.connect(audio, &QAudioInput::notify, [](){
        int bytesReady = audio->bytesReady();
        QByteArray ba(bytesReady, 0);
        audioData->read(ba.data, bytesReady);
        audioFile.write(ba);
    });

    QTimer* timer = new QTimer;
    timer->setInterval(5000);
    timer->start();
    QObject::connect(timer, &QTimer::timeout, [&](){
        audio->stop();
        audio->deleteLater();
        audioFile.close();
        app.quit();
    });

    return app.exec();
}
