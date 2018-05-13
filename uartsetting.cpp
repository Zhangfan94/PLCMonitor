#include "datamenu.h"
#include "uartsetting.h"
#include "ui_uartsetting.h"
#include "GloableData/datatype.h"
#include "GloableData/gloabledata.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>


#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


UartSetting::UartSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UartSetting)
{
    ui->setupUi(this);
}

UartSetting::~UartSetting()
{
    delete ui;
}

bool UartSetting::initMcuSerialService()
{
    //init RTU
    modbus_t *mbTmp ;
    mbTmp = modbus_new_rtu(ui->portCbx->currentText().toLatin1().data(),
                        ui->rateCbx->currentText().toInt(),
                        *(ui->parityCbx->currentText().toLatin1().data()),
                        ui->dataBitCbx->currentText().toInt(),
                        ui->stopBitCbx->currentText().toInt());        //相同的端口只能同时打开一个
    if(NULL == mbTmp)
    {
        qDebug() << "mbTmp serial open failed!";
        return false;
    }

    modbus_set_slave(mbTmp, 1);  //设置modbus从机地址
    modbus_connect(mbTmp);

    struct timeval t;
    t.tv_sec=0;
    t.tv_usec=1000000;   //设置modbus超时时间为1000毫秒
    modbus_set_response_timeout(mbTmp, &t);

    GloableData::GetInstance()->mb = mbTmp;

    McuCommunicateService *mcuService = McuCommunicateService::GetInstance();    //服务线程初始化

    mcuService->start();

    qDebug()<<"MCU SERVICE INIT OK!";
    return true;
}

bool UartSetting::initPlcSerialService()

{

  #if 1
    //init for plc motor
    QSerialPort * serial = new QSerialPort;

    serial->setPortName(ui->portCbx_3->currentText());    //设置串口名
    serial->open(QIODevice::ReadWrite);                   //打开串口
    serial->setBaudRate(ui->rateCbx_3->currentText().toInt());    //设置波特率
    //其他参数暂时写死
    serial->setDataBits(QSerialPort::Data7);        //设置数据位数 7
    serial->setParity(QSerialPort::EvenParity);     //设置奇偶校验 偶
    serial->setStopBits(QSerialPort::OneStop);      //设置停止位 1
    serial->setFlowControl(QSerialPort::NoFlowControl);    //设置流控制 无

    if(!serial->isOpen())
    {
        qDebug() << "plc serial open failed!";
        return false;
    }
    GloableData::GetInstance()->serial= serial;

#endif
   // PlcCommunicateService *plcService = PlcCommunicateService::GetInstance();    //服务线程初始化
  //  plcService->start();
   // qDebug()<<" PLC SERVICE INIT OK!";
    return true;
}

void UartSetting::showEvent(QShowEvent *event)
{

    //初始化控件
 /************************************* 串口：STM32采集 ****************************************************/
    //1、COM口
    auto ports = QSerialPortInfo::availablePorts();
    for (auto port : ports)
    {
        ui->portCbx->addItem(port.portName());
    }
    //2、波特率
    QList<int> rateList;
    rateList << 115200 << 9600 << 34800 << 19200;
    for (auto rate : rateList)
    {
        ui->rateCbx->addItem(QString::number(rate));
    }
    //3、数据位
    QList<int8_t> dataBitList;
    dataBitList << 8 << 7 << 6 <<5;
    for (auto dataBit : dataBitList)
    {
        ui->dataBitCbx->addItem(QString::number(dataBit));
    }
    //4、停止位
    QList<int8_t> stopBitList;
    stopBitList << 1 << 2;
    for (auto stopBit : stopBitList)
    {
        ui->stopBitCbx->addItem(QString::number(stopBit));
    }
    //5、校验位
    QList<QString> parityList;
    parityList << "E" << "O" << "N";
    for ( auto parity : parityList)
    {
        ui->parityCbx->addItem(parity);
    }

   /************************************* 串口：PLC伺服电机 ****************************************************/
    //大多数时间仅仅是修改下 端口和 波特率。我这里在代码中将其他初始化参数写死
    //1、COM口
    auto portsPlc = QSerialPortInfo::availablePorts();
    for (auto portPlc : portsPlc)
    {
        ui->portCbx_3->addItem(portPlc.portName());
    }
    //2、波特率
    QList<int> rateListPlc;
    rateListPlc  << 38400 << 9600 << 115200 << 19200;
    for (auto ratePlc : rateListPlc)
    {
        ui->rateCbx_3->addItem(QString::number(ratePlc));
    }
}


void UartSetting::on_loginBtn_clicked()
{
    #ifdef ONLY_USE_PLC_UART
    if((!initPlcSerialService()))
    #else
    if((!initPlcSerialService()) || (!initMcuSerialService()))
    #endif
    {
       qDebug() << "initSerialPort failed!";
       return;
    }
    dataMenu = new DataMenu();
    dataMenu->show();
    this->hide();
}

void UartSetting::on_retBtn_clicked()
{
    this->close();
}
