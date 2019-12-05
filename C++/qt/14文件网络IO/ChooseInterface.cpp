#include "ChooseInterface.h"
#include <QNetworkInterface>
#include <QVBoxLayout>

ChooseInterface::ChooseInterface(QWidget *parent) :
    QDialog(parent)
{
    /* get all interface */
    QList<QHostAddress> addrList = QNetworkInterface::allAddresses();
#if 0
    QList<QNetworkInterface> infList = QNetworkInterface::allInterfaces();

     QList<QNetworkAddressEntry> entryList = infList.at(0).addressEntries();
     entryList.at(0).broadcast()
#endif

    _comboBox = new QComboBox;
    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->addWidget(_comboBox);

    foreach(QHostAddress addr, addrList)
    {
        quint32 ipaddr = addr.toIPv4Address();
        if(ipaddr == 0)
            continue;
        _comboBox->addItem(QHostAddress(ipaddr).toString());
    }

    connect(_comboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(slotComboBoxChange(QString)));
}

void ChooseInterface::slotComboBoxChange(QString str)
{
    this->_strSelect = str;
}
