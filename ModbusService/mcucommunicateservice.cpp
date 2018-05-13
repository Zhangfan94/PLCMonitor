#include "mcucommunicateservice.h"
#include <QDateTime>
#include <QDebug>
#include "GloableData/gloabledata.h"
#include "libmodbus/modbus.h"
#include <QMessageBox>
#include <QVector>
#include <iostream>

using namespace std;

/*
0x1000（16384）    测到的磁极数
0x1001            N极磁场最大值
0x1002            N极磁场最小值
0x1003            N极磁场平均值
0x1004            S极磁场最大值
0x1005            S极磁场最小值
0x1006            S极磁场平均值
0x1007            实时磁场值AB
0x1008            实时磁场值零位
0x1009            磁场最大值零位
*/
#include <utils.h>
#ifndef LJH_LOG_OUT_TXT(contentString)
#define LJH_LOG_OUT_TXT(contentString) Utils::writeLogTXT(contentString,__FILE__, __LINE__,__FUNCTION__);
#endif

McuCommunicateService::McuCommunicateService()
{
    stopped = false;
}



McuCommunicateService::~McuCommunicateService()
{

}

McuCommunicateService *McuCommunicateService::GetInstance()
{
    static McuCommunicateService* mcuService = NULL;
    if(NULL == mcuService)
    {
        mcuService = new McuCommunicateService;
    }
    return mcuService;
}

void McuCommunicateService::stop()
{
    stopped = true;
}

void McuCommunicateService::addCmd(DataDef::McuCmdType cmd)
{
    mcuCmdQueue.append(cmd);
}

void McuCommunicateService::procDoFunction()
{

}


uint16_t tab_reg[32]={0};
QVector<uint16_t> strArray;
void McuCommunicateService::exec_cmd_mcu_read_register()
{
    //1、发送指令
    //2、等待
    //3、接收返回值
    // qDebug() << "start:" << QDateTime::currentDateTime();
    QString buffer;
    buffer.clear();


    buffer.append("(*+﹏+*)");
    //把所有 点 扔到数组里
    for(int cnt_num=0;cnt_num<1;cnt_num++)
    {                                                           //4096
        int regs=modbus_read_registers(mbMcuCommunicateService, 0x1000, 10, tab_reg);
        for(int i = 0;i<10;i++)
        {
            strArray.append(tab_reg[i]);
            tableWidget->setItem(0,i,new QTableWidgetItem(QString::number(tab_reg[i],16))); //设置表格内容
            // LJH_LOG_OUT_TXT(QString::number(tab_reg[i],16));
            buffer.append(QString::number(tab_reg[i],16));
        }


        // LJH_LOG_OUT_TXT(buffer);
        buffer.clear();
    }
    //  qDebug() << "end:" << QDateTime::currentDateTime();
    tableWidget->update();

#if 0
    //用迭代器show一下
    QVector<uint16_t>::iterator iter;
    for(iter=strArray.begin();iter!=strArray.end();iter++)
    {
        qDebug()<<*iter<<"\0";

    }
    qDebug() << "strArraysize:" <<strArray.size() ;
    qDebug() << "cnt:" <<strArray.size()/10 ;
#endif
}



#define MODBUS_READ_SIZE 10
#define MODBUS_READ_ADDR 0x1000  //4096

void McuCommunicateService::exec_cmd_mcu_read_register2()
{
    uint16_t temp_buff[32]={0};
    QMap<int,ModusBuffDef::RegisterDataStruct> PointBuffMap;  //key 考虑用x轴角度值,value为 当前角度 对应的
    ModusBuffDef::RegisterDataStruct data;
    memset(&data,0,sizeof(data));
    for(int cnt_num=0;cnt_num<1;cnt_num++)
    {
        int regs=modbus_read_registers(mbMcuCommunicateService, MODBUS_READ_ADDR, MODBUS_READ_SIZE, temp_buff);
        memcpy(&data,temp_buff,MODBUS_READ_SIZE*sizeof(uint16_t));
        PointBuffMap.insert(0, data);
    }
    qDebug()<<  PointBuffMap.find(0).value().num_magnetic_pole;
    qDebug()<<  PointBuffMap.find(0).value().value_n_max;
    qDebug()<<  PointBuffMap.find(0).value().value_n_min;
    qDebug()<<  PointBuffMap.find(0).value().value_n_average;
    qDebug()<<  PointBuffMap.find(0).value().value_s_max;
    qDebug()<<  PointBuffMap.find(0).value().value_n_min;
    qDebug()<<  PointBuffMap.find(0).value().value_n_average;
    qDebug()<<  PointBuffMap.find(0).value().value_cur_AB;
    qDebug()<<  PointBuffMap.find(0).value().value_cur_Zero;
    qDebug()<<  PointBuffMap.find(0).value().value_magnetic_field_max_zero;
}

//test to write register
void McuCommunicateService::exec_cmd_mcu_read_register3()
{
    /*
    1.一次读100个点，分 360次for循环 处理完3w6个点
    2.每次读到的100个数据先进行数据转换，转换后的浮点数double 依次append 进 QVector<double>
    3.紧接着将QVector<double> 传入全局的 QVector<QPointF>data 此全局数据可同步在ui主线程 调用定时器绘图 360/36000 = 0.01,
    也就是说每个点间距 时 0.01
    */
    qDebug() << "getpointstart:" << QDateTime::currentDateTime();

    uint16_t temp_buff[36000]={0};
    for(int s = 0; s < 360; s++)
    {
        uint16_t angle[1]={0};
        modbus_set_slave(GloableData::GetInstance()->mb, 1);
        modbus_read_registers( GloableData::GetInstance()->mb, 4096+10, 1, angle);
        qDebug() << angle[0];
        if(angle[0] / 100 < (s + 1))
        {
            if(!((s > 0) && ((angle[0] / 100) == 0)))
            {
                s -= 1;
                continue;
            }

        }

        //波形一
        modbus_set_slave(GloableData::GetInstance()->mb, 2);
        modbus_read_registers( GloableData::GetInstance()->mb, s*100, 100, &temp_buff[s*100]);
        usleep(20);
        for(int i = 0; i<100; i++)
        {
            if(QString::number(temp_buff[s*100 + i], 10).toFloat()>32767)
            {
                QPointF  tempPoint((s*100 + i) * 0.01, (QString::number(temp_buff[s*100 + i], 10).toFloat()-65536)/10.0);
                GloableData::GetInstance()->pointList1.append(tempPoint);
            }
            else
            {
                QPointF  tempPoint((s*100 + i) * 0.01, (QString::number(temp_buff[s*100 + i], 10).toFloat())/10.0);
                GloableData::GetInstance()->pointList1.append(tempPoint);
            }
        }

        //波形二
        modbus_set_slave(GloableData::GetInstance()->mb, 3);
        modbus_read_registers( GloableData::GetInstance()->mb, s*100, 100, &temp_buff[s*100]);
        usleep(20);
        for(int i = 0; i<100; i++)
        {
            if(QString::number(temp_buff[s*100 + i], 10).toFloat()>32767)
            {
                QPointF  tempPoint((s*100 + i) * 0.01, (QString::number(temp_buff[s*100 + i], 10).toFloat()-65536)/10.0);
                GloableData::GetInstance()->pointList2.append(tempPoint);
            }
            else
            {
                QPointF  tempPoint((s*100 + i) * 0.01, (QString::number(temp_buff[s*100 + i], 10).toFloat())/10.0);
                GloableData::GetInstance()->pointList2.append(tempPoint);
            }
        }
    }

    qDebug() << "getpointend:" << QDateTime::currentDateTime();
}


void McuCommunicateService::exec_cmd_mcu_read_state(void)
{
    uint16_t temp_buff[32]={0};
    modbus_set_slave(GloableData::GetInstance()->mb, 1);
    modbus_read_registers(GloableData::GetInstance()->mb, 4096, 10, temp_buff);

    //flush 实时AB磁场
    if(QString::number(temp_buff[7], 10).toFloat()>32767)
    {
        GloableData::GetInstance()->realTimeAB = QString::number((QString::number(temp_buff[7], 10).toFloat()-65536) / 10);
    }
    else
    {
        GloableData::GetInstance()->realTimeAB = QString::number((QString::number(temp_buff[7], 10).toFloat())/10);
    }


    //flush 实时零位磁场
    if(QString::number(temp_buff[8], 10).toFloat()>32767)
    {
        GloableData::GetInstance()->realTimeZero = QString::number((QString::number(temp_buff[8], 10).toFloat()-65536)/10);
    }
    else
    {
        GloableData::GetInstance()->realTimeZero = QString::number((QString::number(temp_buff[8], 10).toFloat())/10);
    }
}


void McuCommunicateService::run()
{

    mbMcuCommunicateService =GloableData::GetInstance()->mb;

    while(!stopped)
    {
        if(mcuCmdQueue.isEmpty())
        {
            usleep(200 * 1000); //等待200毫秒
            continue;
        }
        tableWidget = GloableData::GetInstance()->tableWidget;

        switch(mcuCmdQueue.dequeue())
        {
        case DataDef::READ_POINTS:
            exec_cmd_mcu_read_register3();
            break;
        case DataDef::READ_STATUS:
            exec_cmd_mcu_read_state();
            break;
        default :
            break;
        }

    }
    stopped = false;
}
