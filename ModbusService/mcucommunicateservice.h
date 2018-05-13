#ifndef MCUCOMMUNICATESERVICE_H
#define MCUCOMMUNICATESERVICE_H
#include <stdint.h>
#include <QQueue>
#include <QThread>
#include "cmdtype.h"
#include "libmodbus/modbus.h"
#include <QTableWidget>

namespace ModusBuffDef {

struct RegisterDataStruct
{

public:
    uint16_t num_magnetic_pole;//  测到的磁极数
    uint16_t value_n_max;//  N极磁场最大值
    uint16_t value_n_min;//  N极磁场最小值
    uint16_t value_n_average;//  N极磁场平均值
    uint16_t value_s_max;//  S极磁场最大值
    uint16_t value_s_min;//  S极磁场最小值
    uint16_t value_s_average;//  S极磁场平均值
    uint16_t value_cur_AB;//  实时磁场值AB
    uint16_t value_cur_Zero;//  实时磁场值零位
    uint16_t value_magnetic_field_max_zero;//  磁场最大值零位
};  //这个数据结构 用来整理 modbus 采集的 10个寄存器 数据

}


class McuCommunicateService : public QThread
{
public:
    ~McuCommunicateService();
    static McuCommunicateService *GetInstance();
    void stop();
    void addCmd(DataDef::McuCmdType cmd);
    void procDoFunction();
protected:
    void run();

private:
    McuCommunicateService();


    void exec_cmd_mcu_read_register(void);
    void exec_cmd_mcu_read_register2(void);
    void exec_cmd_mcu_read_register3(void);
    void exec_cmd_mcu_read_state(void);

public:
    QQueue<DataDef::McuCmdType> mcuCmdQueue;
    modbus_t *mbMcuCommunicateService;
private:
    volatile bool stopped;
    QTableWidget *tableWidget;

};

#endif // MCUCOMMUNICATESERVICE_H
