#ifndef PLCCOMMUNICATESERVICE_H
#define PLCCOMMUNICATESERVICE_H

#include <QQueue>
#include <QThread>
#include "cmdtype.h"
#include <QSerialPort>
class PlcCommunicateService : public QThread
{
public:
    ~PlcCommunicateService();
    static PlcCommunicateService *GetInstance();
    void stop();
    bool addCmd(DataDef::PlcCmdType cmd);

protected:
    void run();

private:
    PlcCommunicateService();

public:
    QQueue<DataDef::PlcCmdType> plcCmdQueue;


private:
    volatile bool stopped;
};

#endif // PLCCOMMUNICATESERVICE_H
