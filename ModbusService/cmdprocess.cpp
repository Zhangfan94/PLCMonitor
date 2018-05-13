#include "cmdprocess.h"

/* 电机控制部分协议帧
//write
// 02 30 31 34 32 34 4d 30 30 30 30 30 41 03    电机使能放开        <-    02 30 31 34 32 30 46 39 03
// 02 30 31 34 32 33 4d 30 30 30 30 30 39 03    电机使能锁死        <-    02 30 31 34 32 30 46 39 03
// 02 30 31 34 32 33 4d 30 30 30 31 30 41 03    电机旋转           <-    02 30 31 34 32 30 46 39 03
// 02 30 31 34 32 34 4d 30 30 30 31 30 42 03    电机旋转           <-    02 30 31 34 32 30 46 39 03
// 02 30 31 34 32 33 4d 30 30 30 32 30 42 03    电机一直旋转        <-    02 30 31 34 32 30 46 39 03
// 02 30 31 34 32 34 4d 30 30 30 32 30 43 03    stop



// read
//02 30 31 34 34 30 31 59 30 30 30 32 34 37 03
 anser<-       02 30 31 34 34 30 30 32 42 03    //未使能
               02 30 31 34 34 31 30 32 42 03    //使能


//02 30 31 34 34 30 31 4d 30 30 31 30 33 41 03
 anser <-      02 30 31 34 34 30 30 32 42 03    //没运转
               02 30 31 34 34 30 31 32 43 03    //运转






                    01        47       01      D  R  0  0  0  0   2
  设置转9度   02   30 31    34 37     30 31    44 52 30 30 30 30 32     30 30 30 30 32 33 32 38            34 36 03
                                                                    8个字节  (00002328 -> 9000 -> 9.000)

                    01        46       01      D  R  0  0  0  0   2
  读角度     02    30 31     34 36    30 31    44 52 30 30 30 30 32                                        42 36 03


  anser  <- 02    30 31     34 36                                   30  30 30 30 35 37 45 34 30           41 32 03
                                                                    9个字节 (000057E40 -> 360000 -> 360.000)





                  01          47          01      D  R  0  0  0  0  0
  设置速度    02   30 31     34 37      30 31    44 52 30 30 30 30 30   30 30 30 30 34 36 35 30           34 34 03
                                                                  8个字节 (00004650 -> 18000 -> 18.000/s)

                                                  D  R  0  0  0  0  0
  读速度      02   30 31     34 36      30 31    44 52 30 30 30 30 30                                     42 34 03

  anser <-   02   30 31     34 36                                   30 30 30 30 30 38 43 41 30      41 39 03
                                                                     9个字节(000008CA0  ->  36000  -> 36.000/s)

                        02 30 31 34 36 30 30 30 30 30 38 43 41 30 41 39 03
*/

//write   02 30 31 34 36 30 30 30 30 30 38 43 41 30 41 39 03
static char cmdDataMotorEnableRelase[] = {0x02,0x30,0x31,0x34,0x32,0x34,0x4d,0x30,0x30,0x30,0x30,0x30,0x41,0x03};//电机使能放开
static char cmdDataMotorEnableLocked[] = {0x02,0x30,0x31,0x34,0x32,0x33,0x4d,0x30,0x30,0x30,0x30,0x30,0x39,0x03};//电机使能锁死
static char cmdDataMotorRotateA[] =      {0x02,0x30,0x31,0x34,0x32,0x34,0x4d,0x30,0x30,0x30,0x31,0x30,0x42,0x03};//电机旋转
static char cmdDataMotorRotateB[] =      {0x02,0x30,0x31,0x34,0x32,0x33,0x4d,0x30,0x30,0x30,0x31,0x30,0x41,0x03};//电机旋转
static char cmdDataMotorRotateAllTime[] ={0x02,0x30,0x31,0x34,0x32,0x33,0x4d,0x30,0x30,0x30,0x32,0x30,0x42,0x03};//电机一直旋转
static char cmdDataMotorStop[] =         {0x02,0x30,0x31,0x34,0x32,0x34,0x4d,0x30,0x30,0x30,0x32,0x30,0x43,0x03};//停止旋转



//read
static char cmdDataReadEnableState[] =   {0x02,0x30,0x31,0x34,0x34,0x30,0x31,0x59,0x30,0x30,0x30,0x32,0x34,0x37,0x03};//读 是否使能
static char cmdDataReadRotateState[] =   {0x02,0x30,0x31,0x34,0x34,0x30,0x31,0x4d,0x30,0x30,0x31,0x30,0x33,0x41,0x03};//读 是否旋转
static char cmdDataReadAngle[]=          {0x02,0x30,0x31,0x34,0x36,0x30,0x31,0x44,0x52,0x30,0x30,0x30,0x30,0x32,0x42,0x36,0x03}; //读 角度
static char cmdDataReadSpeed[]=          {0x02,0x30,0x31,0x34,0x36,0x30,0x31,0x44,0x52,0x30,0x30,0x30,0x30,0x30,0x42,0x34,0x03}; //读 转速




// mcu
static char cmdDataMcuReadRegister[]=    {0};

CmdProcess *CmdProcess::GetInstance()
{
    static CmdProcess *cmdProcess = NULL;
    if(NULL == cmdProcess)
    {
        cmdProcess = new CmdProcess;
    }
    return cmdProcess;
}

QMap<DataDef::PlcCmdType, DataDef::DataStruct> CmdProcess:: getPlcMap()
{
  return this->plcCmdMap;
}

CmdProcess::CmdProcess()
{
    plcCmdMap.clear();
    plcCmdMap.clear();
    CmdInit();
}

void CmdProcess::CmdInit()
{
    //注册PLC命令
    PlcCmdInit();
    //注册MCU命令
    McuCmdInit();
}

void CmdProcess::PlcCmdInit()
{
    CmdRegister(DataDef::C_ENABLE_RELEASE, DataDef::DataStruct((char *)cmdDataMotorEnableRelase, sizeof(cmdDataMotorEnableRelase) / sizeof(char)));
    CmdRegister(DataDef::C_ENABLE_LOCKED, DataDef::DataStruct((char*)cmdDataMotorEnableLocked, sizeof(cmdDataMotorEnableLocked) / sizeof(char)));
    CmdRegister(DataDef::C_ROTATE_A, DataDef::DataStruct((char *)cmdDataMotorRotateA, sizeof(cmdDataMotorRotateA) / sizeof(char)));
    CmdRegister(DataDef::C_ROTATE_B, DataDef::DataStruct((char*)cmdDataMotorRotateB, sizeof(cmdDataMotorRotateB) / sizeof(char)));
    CmdRegister(DataDef::C_ROTATE_ALL_TIME, DataDef::DataStruct((char*)cmdDataMotorRotateAllTime, sizeof(cmdDataMotorRotateAllTime) / sizeof(char)));
    CmdRegister(DataDef::C_STOP, DataDef::DataStruct((char*)cmdDataMotorStop, sizeof(cmdDataMotorStop) / sizeof(char)));
    CmdRegister(DataDef::C_READ_STATE_ENABLE, DataDef::DataStruct((char*)cmdDataReadEnableState, sizeof(cmdDataReadEnableState) / sizeof(char)));
    CmdRegister(DataDef::C_READ_STATE_ROTATE, DataDef::DataStruct((char*)cmdDataReadRotateState, sizeof(cmdDataReadRotateState) / sizeof(char)));
    CmdRegister(DataDef::C_READ_ANGLE, DataDef::DataStruct((char*)cmdDataReadAngle, sizeof(cmdDataReadAngle) / sizeof(char)));
    CmdRegister(DataDef::C_READ_SPEED, DataDef::DataStruct((char*)cmdDataReadSpeed, sizeof(cmdDataReadSpeed) / sizeof(char)));

   // ................

}

void CmdProcess::McuCmdInit()
{

     CmdRegister(DataDef::READ_POINTS, DataDef::DataStruct((char*)cmdDataMcuReadRegister, sizeof(cmdDataMcuReadRegister) / sizeof(char)));
}

void CmdProcess::CmdRegister(DataDef::PlcCmdType cmd, DataDef::DataStruct data)
{
    if(plcCmdMap.end() != plcCmdMap.find(cmd)) return;

    plcCmdMap.insert(cmd, data);


}

void CmdProcess::CmdRegister(DataDef::McuCmdType cmd, DataDef::DataStruct data)
{
    if(mcuCmdMap.end() != mcuCmdMap.find(cmd)) return;

    mcuCmdMap.insert(cmd, data);
}
