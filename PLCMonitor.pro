#-------------------------------------------------
#
# Project created by QtCreator 2018-03-12T22:22:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET += PLCMonitor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG+=qaxcontainer
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -Ldll -lws2_32

QT += serialport
QT += charts
QT += widgets
QT += sql


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    libmodbus/modbus-data.c \
    libmodbus/modbus-rtu.c \
    libmodbus/modbus-tcp.c \
    libmodbus/modbus.c \
    datamenu.cpp \
    settingmenu.cpp \
    uartsetting.cpp \
    GloableData/gloabledata.cpp \
    callout.cpp \
    chartview.cpp \
    ModbusService/mcucommunicateservice.cpp \
    ModbusService/plccommunicateservice.cpp \
    ModbusService/cmdprocess.cpp \
    utils.cpp \
    execltool/excelengine.cpp \
    execltool/qexcel.cpp \
    demotest.cpp \
    databasetool/databaseengine.cpp

HEADERS += \
    mainwindow.h \
    libmodbus/config.h \
    libmodbus/modbus-private.h \
    libmodbus/modbus-rtu-private.h \
    libmodbus/modbus-rtu.h \
    libmodbus/modbus-tcp-private.h \
    libmodbus/modbus-tcp.h \
    libmodbus/modbus-version.h \
    libmodbus/modbus.h \
    datamenu.h \
    settingmenu.h \
    uartsetting.h \
    GloableData/gloabledata.h \
    GloableData/datatype.h \
    callout.h \
    chartview.h \
    ModbusService/mcucommunicateservice.h \
    ModbusService/plccommunicateservice.h \
    ModbusService/cmdprocess.h \
    ModbusService/cmdtype.h \
    macro_define.h \
    utils.h \
    execltool/excelengine.h \
    execltool/qexcel.h \
    databasetool/databaseengine.h

FORMS += \
    mainwindow.ui \
    datamenu.ui \
    settingmenu.ui \
    uartsetting.ui

RESOURCES += \
    mypic.qrc


RC_FILE = monitor_logo.rc


#TARGET = QExcel
#CONFIG   += console
#CONFIG   -= app_bundle






