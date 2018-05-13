#include "gloabledata.h"

GloableData::GloableData()
{
    mb = NULL;
    serial = NULL;
    tableWidget = NULL;
    tableWidget_2 = NULL;

    g_database = NULL;
    //命令初始化
    cmdProcess = CmdProcess::GetInstance();
}

GloableData::~GloableData()
{
    delete mb;
    delete serial;
    delete tableWidget;
    delete tableWidget_2;

     delete g_database;
}

GloableData *GloableData::GetInstance()
{
    static GloableData * gloableData = NULL;
    if(NULL == gloableData)
    {
        gloableData = new GloableData();
    }
    return gloableData;
}
