#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtDebug>
#include <QMessageBox>
#include <QSerialPort>

#include "MyEepromAddresses.h"
#include "MyConfigFlea.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mSerialPort(*new QSerialPort(this))
{
    ui->setupUi(this);
    QList<QSerialPortInfo>	ports = QSerialPortInfo::availablePorts();
    for (int i = 0; i < ports.size(); i++)
    {
        //ui->portsPresents->addItem(ports.at(i).portName());
        ui->portsPresents->addItem(ports.at(i).systemLocation());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    mEepromContent.clear();
    mSerialPort.write("r\n");
}

void MainWindow::displayEeprom()
{
    ui->display->clear();
    QString text;
    for (int i = 0; i < mEepromContent.size(); i++)
    {
        text.append(trUtf8("0x%1, ").arg((quint8)mEepromContent.at(i), 2, 16, QChar('0')));
    }
    ui->display->setText(text);
}

void MainWindow::eepromToGui()
{
    if (mEepromContent.size() >= TailleConfiguration)
    {
        if (mEepromContent.at(PositionVersionConfig) == 1)
        {
            ui->rtc->setChecked(mEepromContent.at(PositionConfigRtc) != 0);
            ui->temperature->setChecked(mEepromContent.at(PositionConfigTemperature) != 0);
            ui->humidite->setChecked(mEepromContent.at(PositionConfigHumidity) != 0);
            ui->pression->setChecked(mEepromContent.at(PositionConfigPressure) != 0);
            ui->LCD->setChecked(mEepromContent.at(PositionConfigAdresseLcd) != 0);
            ui->adresseLcd->setValue((quint8)mEepromContent.at(PositionConfigAdresseLcd));
            //ui->rtc->setChecked(mEepromContent.at(PositionConfigTypeLcd) != 0);
            if (mEepromContent.at(PositionConfigTypeLcd) == LCD_2004)
            {
                ui->typeLcd->setCurrentText("20x4");
            }
            else
            {
                ui->typeLcd->setCurrentText("16x2");
            }
        }
        else
        {
            statusBar()->showMessage(trUtf8("Version %1 non supportée").arg((quint8)mEepromContent.at(PositionVersionConfig)));
        }
        if (mEepromContent.size() >= TailleConfiguration + 1)
        {
            // NodeId
            ui->nodeId->setValue(mEepromContent.at(TailleConfiguration));
        }
    }
    else
    {
        statusBar()->showMessage(trUtf8("Taille eeprom insuffisante : %1 < %2").arg(mEepromContent.size()).arg(TailleConfiguration));
    }
}

void MainWindow::guiToEeprom()
{
    if (mEepromContent.size() > TailleConfiguration)
    {
        mEepromContent[PositionVersionConfig] = 1;
        mEepromContent[PositionConfigRtc] = (ui->rtc->isChecked() ? 1 : 0);
        mEepromContent[PositionConfigTemperature] = (ui->temperature->isChecked() ? 1 : 0);
        mEepromContent[PositionConfigHumidity] = (ui->humidite->isChecked() ? 1 : 0);
        mEepromContent[PositionConfigPressure] = (ui->pression->isChecked() ? 1 : 0);
        if (ui->LCD->isChecked())
        {
            mEepromContent[PositionConfigAdresseLcd] = (quint8)ui->adresseLcd->value();
            if (ui->typeLcd->currentText() == "20x4")
            {
                mEepromContent[PositionConfigTypeLcd] = LCD_2004;
            }
            else
            {
                mEepromContent[PositionConfigTypeLcd] = LCD_1602;
            }
        }
        else
        {
            mEepromContent[PositionConfigAdresseLcd] = 0;
        }
    }
    else
    {
        statusBar()->showMessage(trUtf8("Taille eeprom insuffisante : %1 < %2").arg(mEepromContent.size()).arg(TailleConfiguration));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    guiToEeprom();
    displayEeprom();
    if (mSerialPort.isOpen())
    {
        QString chaine = "W";
        for (int i = 0; i < mEepromContent.size(); i++)
        {
            chaine += trUtf8(" %1").arg((quint8)mEepromContent.at(i));
        }
        chaine.append("\n");
        qDebug() << chaine;
        mSerialPort.write(chaine.toLatin1());
    }
    else
    {
        statusBar()->showMessage(trUtf8("Port %1 non ouvert").arg(ui->portsPresents->currentText()));
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if (QMessageBox::warning(this, tr("Weather configuration"),
                                     tr("L'eeprom va être efacée.\n"
                                        "Etes-vous sûr ?"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Cancel) == QMessageBox::Ok)
    {
        statusBar()->showMessage(trUtf8("Effacement..."));
    }
}

void MainWindow::on_connecter_clicked()
{
    if (!mSerialPort.isOpen())
    {
        mSerialPort.setPortName(ui->portsPresents->currentText());
        if (mSerialPort.open(QIODevice::ReadWrite))
        {
            statusBar()->showMessage(trUtf8("%1 ouvert").arg(ui->portsPresents->currentText()));
            mSerialPort.setBaudRate(115200);
            connect(&mSerialPort, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
        }
        else
        {
            statusBar()->showMessage(trUtf8("Impossible d'ouvrir %1").arg(ui->portsPresents->currentText()));
        }
    }
}

void MainWindow::on_deconnecter_clicked()
{
    if (mSerialPort.isOpen())
    {
        mSerialPort.close();
        statusBar()->showMessage(trUtf8("%1 fermé").arg(ui->portsPresents->currentText()));
    }
}

// This slot is connected to QSerialPort::readyRead()
void MainWindow::readyReadSlot()
{
    while (!mSerialPort.atEnd())
    {
        QByteArray data = mSerialPort.readAll();
        qDebug() << data;
        if (data.at(0) == 'C')
        {
            QStringList contenu = QString(data).split(QChar(' '));
            for (int i = 1; i < contenu.size(); i++)
            {
                bool ok;
                quint8 val = contenu.at(i).toUShort(&ok);
                if (ok)
                {
                    mEepromContent.append(val);
                }
            }
        }
    }
    displayEeprom();
    eepromToGui();
}

void MainWindow::on_pushButton_5_clicked()
{

}

void MainWindow::on_pushButton_clicked()
{

}
