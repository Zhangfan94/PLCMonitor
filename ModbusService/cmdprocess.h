#ifndef CMDPROCESS_H
#define CMDPROCESS_H

#include "cmdtype.h"
#include <QMap>

//这个类用于函数的命令号注册，将命令好与对应的指令绑定起来；

class CmdProcess
{
public:
    static CmdProcess *GetInstance();
     QMap<DataDef::PlcCmdType,DataDef::DataStruct> getPlcMap();
private:
    CmdProcess();
    void CmdInit();
    void PlcCmdInit();
    void McuCmdInit();
    void CmdRegister(DataDef::PlcCmdType cmd, DataDef::DataStruct data);
    void CmdRegister(DataDef::McuCmdType cmd, DataDef::DataStruct data);

private:
    QMap<DataDef::PlcCmdType,DataDef::DataStruct> plcCmdMap;
    QMap<DataDef::McuCmdType,DataDef::DataStruct> mcuCmdMap;

};

#endif // CMDPROCESS_H
