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

private:
    typedef enum
    {
        LCD_1602,
        LCD_2004
    }
    eTypeLcd;

    Ui::MainWindow *ui;

    QByteArray mEepromContent;

    void displayEeprom();
    void eepromToGui();
    void guiToEeprom();

    QSerialPort& mSerialPort;
};

#endif // MAINWINDOW_H
