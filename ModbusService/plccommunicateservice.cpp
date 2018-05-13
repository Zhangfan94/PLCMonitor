#include "plccommunicateservice.h"
#include <QDateTime>
#include <QString>
#include <QDebug>
#include "GloableData/gloabledata.h"

PlcCommunicateService::PlcCommunicateService()
{
    stopped = false;

}

PlcCommunicateService::~PlcCommunicateService()
{

}

PlcCommunicateService *PlcCommunicateService::GetInstance()
{
    static PlcCommunicateService *plcService = NULL;
    if(NULL == plcService)
    {
        plcService = new PlcCommunicateService;
    }
    return plcService;
}

void PlcCommunicateService::stop()
{
    stopped = true;
}

bool PlcCommunicateService::addCmd(DataDef::PlcCmdType cmd)
{
    plcCmdQueue.append(cmd);
}

void PlcCommunicateService::run()
{

    while(!stopped)
    {
        if(plcCmdQueue.isEmpty())
        {
            usleep(200 * 1000); //等待200毫秒
            continue;
        }
        qDebug() << plcCmdQueue.dequeue();
        qDebug() << "This is PlcCommunicateService!!" << QDateTime::currentDateTime();
        {
            //1、发送指令
            qDebug()<<"....PlcCommunicateService::run()";


            //2、等待

            //3、接收返回值

            //4、解析
        }
    }
    stopped = false;
}
