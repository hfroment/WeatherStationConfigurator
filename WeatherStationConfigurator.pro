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
    ../../Arduino/libraries/MySensors-master/MyConfigFlea.h \
    ../../Arduino/libraries/MySensors-master/core/MyEepromAddresses.h \
    ../../Arduino/libraries/WeatherStation/WeatherStation.h

INCLUDEPATH += ../../Arduino/libraries/MySensors-master \
../../Arduino/libraries/MySensors-master/core \
    ../../Arduino/libraries/WeatherStation

FORMS    += mainwindow.ui
