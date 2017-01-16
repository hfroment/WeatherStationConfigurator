#-------------------------------------------------
#
# Project created by QtCreator 2017-01-13T21:47:28
#
#-------------------------------------------------

QT       += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeatherStationConfigurator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ../MySensors-master/MyConfigFlea.h \
    ../MySensors-master/core/MyEepromAddresses.h \
    ../WeatherStation/WeatherStation.h

INCLUDEPATH += ../MySensors-master \
../MySensors-master/core \
    ../WeatherStation

FORMS    += mainwindow.ui
