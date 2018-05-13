#ifndef CMDTYPE_H
#define CMDTYPE_H

namespace DataDef {

enum PlcCmdType{
    C_ENABLE_RELEASE = 0,   //cmdDataMotorEnableRelase
    C_ENABLE_LOCKED,        //cmdDataMotorEnableLocked
    C_ROTATE_A,             //cmdDataMotorRotateA
    C_ROTATE_B,             //cmdDataMotorRotateB
    C_ROTATE_ALL_TIME,      //cmdDataMotorRotateAllTime
    C_STOP,                 //cmdDataMotorStop
    C_READ_STATE_ENABLE,    //cmdDataReadEnableState
    C_READ_STATE_ROTATE,    //cmdDataReadRotateState
    C_READ_SPEED,           //cmdDataReadSpeed
    C_READ_ANGLE,           //cmdDataReadAngle
};

enum McuCmdType{
    TEST11,
    READ_POINTS,
    READ_STATUS,
};


struct DataStruct
{
public:
    int dataLen;
    char *data;
    DataStruct(char* data, int dataLen) {
        this->data = data;
        this->dataLen = dataLen;
    }
};

}

#endif // CMDTYPE_H
