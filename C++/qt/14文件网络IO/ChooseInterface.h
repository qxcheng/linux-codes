#ifndef CHOOSEINTERFACE_H
#define CHOOSEINTERFACE_H

#include <QDialog>
#include <QComboBox>

class ChooseInterface : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseInterface(QWidget *parent = 0);
    QComboBox* _comboBox;
    QString _strSelect;
signals:

public slots:

    void slotComboBoxChange(QString);

};

#endif // CHOOSEINTERFACE_H
