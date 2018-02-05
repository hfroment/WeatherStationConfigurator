#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QSerialPort;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_connecter_clicked();

    void on_deconnecter_clicked();

    void readyReadSlot();

    void on_pushButton_5_clicked();

    void on_pushButton_clicked();

private:
    typedef enum
    {
        LCD_1602,
        LCD_2004,
        LCD_SSD1306
    }
    eTypeLcd;

    enum dhtmodels {
        DHT11 = 1,
        DHT22,
        DHT33,
        DHT44,
        DHT12,
    };

    Ui::MainWindow *ui;

    QByteArray mEepromContent;

    void displayEeprom();
    void eepromToGui();
    void guiToEeprom();

    QSerialPort& mSerialPort;
};

#endif // MAINWINDOW_H
