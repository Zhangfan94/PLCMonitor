#ifndef GLOABLEDATA_H
#define GLOABLEDATA_H

#include "datatype.h"
#include "libmodbus/modbus.h"
#include <ModbusService/cmdprocess.h>

#include <QSerialPort>

#include <ModbusService/mcucommunicateservice.h>
#include <ModbusService/plccommunicateservice.h>
#include <QTableWidget>

#include "../databasetool/databaseengine.h"
class GloableData
{

public:
    ~GloableData();
    static GloableData* GetInstance();

private:
    GloableData();

public:
    modbus_t *mb;
    QSerialPort* serial;

    CmdProcess *cmdProcess;
    McuCommunicateService *mcuService;
    PlcCommunicateService *plcService;

    QTableWidget *tableWidget;
    QTableWidget *tableWidget_2;

    QList<QPointF> pointList1;
    QList<QPointF> pointList2;

    QString realTimeAB;
    QString realTimeZero;

    MYDataBase * g_database;

};

#endif // GLOABLEDATA_H
