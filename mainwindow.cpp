#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtDebug>

#include "MyEepromAddresses.h"
#include "MyConfigFlea.h"
#include "WeatherStation.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QSerialPortInfo>	ports = QSerialPortInfo::availablePorts();
    for (int i = 0; i < ports.size(); i++)
    {
        ui->portsPresents->addItem(ports.at(i).systemLocation());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    mEepromContent.fill(0xff, 512);
    displayEeprom();
    eepromToGui();
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
    if (mEepromContent.size() > TailleConfiguration)
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
        }
        else
        {
            statusBar()->showMessage(trUtf8("Version %1 non supportée").arg((quint8)mEepromContent.at(PositionVersionConfig)));
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
                mEepromContent[PositionConfigTypeLcd] = WeatherStation::LCD_2004;
            }
            else
            {
                mEepromContent[PositionConfigTypeLcd] = WeatherStation::LCD_1602;
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
}
