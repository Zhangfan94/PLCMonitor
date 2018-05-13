#include "datamenu.h"
#include "ui_datamenu.h"
#include <QDateTime>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtMath>
#include "ModbusService/mcucommunicateservice.h"
#include "ModbusService/plccommunicateservice.h"
#include <QPen>
#include <QColor>
#include <QPainter>
#include "GloableData/gloabledata.h"
#include "./ModbusService/cmdtype.h"
#include "utils.h"
/*******************************************/
#include "uartsetting.h"
#include "ui_uartsetting.h"
#include "GloableData/datatype.h"
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QTextStream>
#include <QDateTime>
#include "./execltool/excelengine.h"
#include "qt_windows.h"
#include <QVariant>
#include <Qdebug>
#include <QScrollBar>



DataMenu::DataMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataMenu),
    chart(new QChart),
    tip(0),
    timerChart(new QTimer),
    timer_updatastatue(new QTimer),
    count(0),
    flagUartOpen(0),
    flagUartOpenMcu(0),
    flagUartOpenPlc(0),
    isStopping(false)
{
    ui->setupUi(this);

    //UI container init,  某些信号注册函数 我也写在里面了

    initOtherValue();
    initChart();
    initTableWidget();
    initTableWidget_2();
    initSerialPortBox();
    initRadioButtonUart();

    // register signal
    connect(timerChart, SIGNAL(timeout()),              this,              SLOT(timerSlot()));
    connect(series1,     SIGNAL(hovered(QPointF, bool)), this,              SLOT(tipSlot(QPointF,bool)));
    connect(&timerDate, SIGNAL(timeout()),              this,              SLOT(refreshData()));

    connect(timer_updatastatue, SIGNAL(timeout()),              this,              SLOT(updatastau2()));

    this->setWindowTitle("主菜单");




    double xx = 100.00;
    for(int i = 0 ;i<36000;i++)
    {
        test_data.append(QPointF(i/100.00, 20.16));
    }

    //   qDebug()<<test_data.size();

    ui->label_13->hide();
}

DataMenu::~DataMenu()
{
    delete ui;
}


void DataMenu::showEvent(QShowEvent *event)
{
    timerDate.start(200);

    timerChart->setInterval(30);
}


void DataMenu::paintEvent(QPaintEvent *event)
{
    initDataMenuWnd();
}


void DataMenu::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        chart->zoom(1.1);
    }
    else
    {
        chart->zoom(10.0/11);
    }
    QWidget::wheelEvent(event);
}





void DataMenu::serialPlcReciveMannage()
{
    QByteArray temp = GloableData::GetInstance()->serial->readAll();

    if(!temp.isEmpty())
    {
        byteArrayReciveBuff.append(temp);
        if(byteArrayReciveBuff.contains(0x03))       //ack 收完了,!!!这里可能需要超时处理，偶然未接受到03继续缓存的问题。
        {
            {
                //  lrc ..........
                if(1!=Utils::lrcCheck(byteArrayReciveBuff))
                {

                    qDebug()<<"lrc error check!!";
                    byteArrayReciveBuff.clear();
                    return;
                }
            }

            qDebug()<<"datamenu recive"<<byteArrayReciveBuff.data();

            // 这里需要加入一个解析器处理
            if(   byteArrayReciveBuff.at(3) ==0x34 && byteArrayReciveBuff.at(4) ==0x32 )
            {
                if(byteArrayReciveBuff.at(5) == 0x30)
                {
                    qDebug()<< "ACK OK!";
                }
            }

            byteArrayReciveBuff.clear();
        }
    }

}




bool DataMenu::initPlcSerialService()
{

    //init for plc motor
    QSerialPort * serial = new QSerialPort;

    serial->setPortName(ui->portCbx_plc->currentText());    //设置串口名
    serial->open(QIODevice::ReadWrite);                   //打开串口
    serial->setBaudRate(ui->rateCbx_plc->currentText().toInt());    //设置波特率
    //其他参数暂时写死
    serial->setDataBits(QSerialPort::Data7);        //设置数据位数 7
    serial->setParity(QSerialPort::EvenParity);     //设置奇偶校验 偶
    serial->setStopBits(QSerialPort::OneStop);      //设置停止位 1
    serial->setFlowControl(QSerialPort::NoFlowControl);    //设置流控制 无

    if(!serial->isOpen())
    {
        qDebug() << "plc serial open failed!";
        QMessageBox::about(NULL, "报告", QString("串口打开失败，请重启！"));
        this->close();
        return false;
    }
    GloableData::GetInstance()->serial= serial; //初始化完了 扔到全局里面

#if 0    //是否需要plc 线程
    PlcCommunicateService *plcService = PlcCommunicateService::GetInstance();    //服务线程初始化
    plcService->start();
    qDebug()<<" PLC SERVICE INIT OK!";
#endif

    //enable plc read signal
    connect(GloableData::GetInstance()->serial, SIGNAL(readyRead()), this, SLOT(serialPlcReciveMannage()));
    return true;
}

bool DataMenu::initMcuSerialService()
{
    //init RTU
    modbus_t *mbTmp ;
//    mbTmp = modbus_new_rtu(ui->portCbx_stm->currentText().toLatin1().data(),
//                           ui->rateCbx_stm->currentText().toInt(),
//                           *(ui->parityCbx_stm->currentText().toLatin1().data()),
//                           ui->dataBitCbx_stm->currentText().toInt(),
//                           ui->stopBitCbx_stm->currentText().toInt());        //相同的端口只能同时打开一个

        mbTmp = modbus_new_rtu(ui->portCbx_stm->currentText().toLatin1().data(),
                               256000,
                               *(ui->parityCbx_stm->currentText().toLatin1().data()),
                               ui->dataBitCbx_stm->currentText().toInt(),
                               ui->stopBitCbx_stm->currentText().toInt());        //相同的端口只能同时打开一个


    if(NULL == mbTmp)
    {
        qDebug() << "mbTmp serial open failed!";
        QMessageBox::about(NULL, "报告", QString("串口打开失败，请重启！"));
        this->close();
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




void DataMenu::initDataMenuWnd()
{

}
void DataMenu::initChart()
{
    series1 = new QLineSeries;
    series1->setColor(Qt::red);
    chart->addSeries(series1);

    series2 = new QLineSeries;
    series2->setColor(Qt::blue);
    chart->addSeries(series2);

    //  series->setUseOpenGL(true);
    chart->createDefaultAxes();

    chart->axisY()->setRange(-80, 80);
    chart->axisX()->setRange(0, 360);

    chart->axisX()->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    chart->axisY()->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));

    chart->axisX()->setTitleText("角度");
    chart->axisY()->setTitleText("磁场强度");

    chart->axisX()->setGridLineVisible(true);
    chart->axisY()->setGridLineVisible(true);

    chart->setTheme(QChart::ChartThemeLight);

    /* hide legend */
    chart->legend()->hide();
    chartView = new ChartView(chart);

    chartView->setRenderHint(QPainter::Antialiasing);

    ui->horizontalLayout_chartview->addWidget(chartView);
}




void DataMenu::initTableWidget()
{
    ui->tableWidget->setRowCount(1000); //设置行数
    ui->tableWidget->setColumnCount(18); //设置列数
    // ui-> tableWidget->setHorizontalHeaderLabels(QStringList());
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(65);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(true);

    ui->tableWidget->setSpan(0,0,2,1);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("序号"));

    ui->tableWidget->setSpan(0,1,2,1);
    ui->tableWidget->setItem(0,1,new QTableWidgetItem("磁极数"));


    ui->tableWidget->setSpan(0,2,1,2);
    ui->tableWidget->setItem(0,2,new QTableWidgetItem("磁场最大值(mT)"));

    ui->tableWidget->setSpan(0,4,1,2);
    ui->tableWidget->setItem(0,4,new QTableWidgetItem("磁场最小值(mT)"));

    ui->tableWidget->setSpan(0,6,1,2);
    ui->tableWidget->setItem(0,6,new QTableWidgetItem("磁场平均值(mT)"));

    ui->tableWidget->setSpan(0,8,2,2);
    ui->tableWidget->setItem(0,8,new QTableWidgetItem("磁场偏差(mT)"));

    ui->tableWidget->setSpan(0,10,1,2);
    ui->tableWidget->setItem(0,10,new QTableWidgetItem("宽度最大值(mT)"));

    ui->tableWidget->setSpan(0,12,1,2);
    ui->tableWidget->setItem(0,12,new QTableWidgetItem("宽度最小值(mT)"));

    ui->tableWidget->setSpan(0,14,1,2);
    ui->tableWidget->setItem(0,14,new QTableWidgetItem("宽度平均值(mT)"));

    ui->tableWidget->setSpan(0,16,2,2);
    ui->tableWidget->setItem(0,16,new QTableWidgetItem("宽度偏差(mm)"));

    ui->tableWidget->setItem(1,2,new QTableWidgetItem("N"));
    ui->tableWidget->setItem(1,3,new QTableWidgetItem("S"));
    ui->tableWidget->setItem(1,4,new QTableWidgetItem("N"));
    ui->tableWidget->setItem(1,5,new QTableWidgetItem("S"));
    ui->tableWidget->setItem(1,6,new QTableWidgetItem("N"));
    ui->tableWidget->setItem(1,7,new QTableWidgetItem("S"));

    ui->tableWidget->setItem(1,10,new QTableWidgetItem("N"));
    ui->tableWidget->setItem(1,11,new QTableWidgetItem("S"));
    ui->tableWidget->setItem(1,12,new QTableWidgetItem("N"));
    ui->tableWidget->setItem(1,13,new QTableWidgetItem("S"));
    ui->tableWidget->setItem(1,14,new QTableWidgetItem("N"));
    ui->tableWidget->setItem(1,15,new QTableWidgetItem("S"));

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
    //    QStringList header;
    //    header<<tr("测到的磁极数")<<tr("N极磁场最大值")<<tr("N极磁场最小值")<<tr("N极磁场平均值")<<tr("S极磁场最大值")<<tr("S极磁场最小值")<<tr("S极磁场平均值")<<tr("实时磁场值AB")<<tr("实时磁场值零位")<<tr("磁场最大值零位");
    //    ui->tableWidget->setHorizontalHeaderLabels(header);

    GloableData::GetInstance()->tableWidget = ui->tableWidget;



}

void DataMenu::initTableWidget_2()
{

    ui->tableWidget_2->setRowCount(1000); //设置行数
    ui->tableWidget_2->setColumnCount(11); //设置列数
    // ui-> tableWidget->setHorizontalHeaderLabels(QStringList());
    ui->tableWidget_2->horizontalHeader()->setDefaultSectionSize(65);
    ui->tableWidget_2->horizontalHeader()->setVisible(false);
    ui->tableWidget_2->verticalHeader()->setVisible(true);

    ui->tableWidget_2->setSpan(0,0,2,1);
    ui->tableWidget_2->setItem(0,0,new QTableWidgetItem("极数"));

    ui->tableWidget_2->setSpan(0,1,1,2);
    ui->tableWidget_2->setItem(0,1,new QTableWidgetItem("磁场强度(mT)"));

    ui->tableWidget_2->setSpan(0,3,1,2);
    ui->tableWidget_2->setItem(0,3,new QTableWidgetItem("磁极宽度(mm)"));

    ui->tableWidget_2->setSpan(0,5,1,2);
    //  ui->tableWidget_2->setItem(0,5,new QTableWidgetItem("最大磁场："));
    QTableWidgetItem *item = new QTableWidgetItem("最大磁场：");
    item->setBackgroundColor(QColor(128,255,128));
    item->setTextColor(QColor(255,0,0));
    item->setFont(QFont("Helvetica"));
    ui->tableWidget_2->setItem(0,5,item);



    ui->tableWidget_2->setSpan(1,5,1,2);
    ui->tableWidget_2->setItem(1,5,new QTableWidgetItem("最小磁场："));
    ui->tableWidget_2->setSpan(2,5,1,2);
    ui->tableWidget_2->setItem(2,5,new QTableWidgetItem("最大极宽："));
    ui->tableWidget_2->setSpan(3,5,1,2);
    ui->tableWidget_2->setItem(3,5,new QTableWidgetItem("最小极宽："));

    ui->tableWidget_2->setSpan(0,8,1,2);
    ui->tableWidget_2->setItem(0,8,new QTableWidgetItem("平均值："));
    ui->tableWidget_2->setSpan(1,8,1,2);
    ui->tableWidget_2->setItem(1,8,new QTableWidgetItem("偏差值："));
    ui->tableWidget_2->setSpan(2,8,1,2);
    ui->tableWidget_2->setItem(2,8,new QTableWidgetItem("平均值："));
    ui->tableWidget_2->setSpan(3,8,1,2);
    ui->tableWidget_2->setItem(3,8,new QTableWidgetItem("偏差值："));


    ui->tableWidget_2->setItem(1,1,new QTableWidgetItem("N"));
    ui->tableWidget_2->setItem(1,2,new QTableWidgetItem("S"));
    ui->tableWidget_2->setItem(1,3,new QTableWidgetItem("N"));
    ui->tableWidget_2->setItem(1,4,new QTableWidgetItem("S"));


    GloableData::GetInstance()->tableWidget_2 = ui->tableWidget_2;



}

void DataMenu::initOtherValue()
{
    qcmdProcess = CmdProcess::GetInstance();
    tempPlcMap =qcmdProcess->getPlcMap();

    timeCount = 0;
    runTime = 0;

    this->setMinimumSize(1420,900);
    this->setMaximumSize(1420,900);
    byteArrayReciveBuff.clear();
    ui->dial->setValue(1);
}

void DataMenu::initSerialPortBox()
{
    //初始化控件
    /************************************* 串口：STM32采集 ****************************************************/
    //1、COM口
    auto ports = QSerialPortInfo::availablePorts();
    for (auto port : ports)
    {
        ui->portCbx_stm->addItem(port.portName());
    }
    //2、波特率
    QList<int> rateList;
    rateList << 115200 << 9600 << 34800 << 19200;
    for (auto rate : rateList)
    {
        ui->rateCbx_stm->addItem(QString::number(rate));
    }
    //3、数据位
    QList<int8_t> dataBitList;
    dataBitList << 8 << 7 << 6 <<5;
    for (auto dataBit : dataBitList)
    {
        ui->dataBitCbx_stm->addItem(QString::number(dataBit));
    }
    //4、停止位
    QList<int8_t> stopBitList;
    stopBitList << 1 << 2;
    for (auto stopBit : stopBitList)
    {
        ui->stopBitCbx_stm->addItem(QString::number(stopBit));
    }
    //5、校验位
    QList<QString> parityList;
    parityList << "E" << "O" << "N";
    for ( auto parity : parityList)
    {
        ui->parityCbx_stm->addItem(parity);
    }

    /************************************* 串口：PLC伺服电机 ****************************************************/
    //大多数时间仅仅是修改下 端口和 波特率。我这里在代码中将其他初始化参数写死
    //1、COM口
    auto portsPlc = QSerialPortInfo::availablePorts();
    for (auto portPlc : portsPlc)
    {
        ui->portCbx_plc->addItem(portPlc.portName());
    }
    //2、波特率
    QList<int> rateListPlc;
    rateListPlc  << 38400 << 9600 << 115200 << 19200;
    for (auto ratePlc : rateListPlc)
    {
        ui->rateCbx_plc->addItem(QString::number(ratePlc));
    }
}

void DataMenu::initRadioButtonUart()
{

    btnGroupUartMcu = new QButtonGroup(this);
    btnGroupUartMcu->addButton(ui->radioButtonMcuClose, RADIOBUTTON_CLOSE_MCU);
    btnGroupUartMcu->addButton(ui->radioButtonMcuOpen, RADIOBUTTON_OPEN_MCU);

    ui->radioButtonMcuClose->setChecked(true);

    btnGroupUartPlc = new QButtonGroup(this);
    btnGroupUartPlc->addButton(ui->radioButtonPlcClose, RADIOBUTTON_CLOSE_PLC);
    btnGroupUartPlc->addButton(ui->radioButtonPlcOpen, RADIOBUTTON_OPEN_PLC);

    ui->radioButtonPlcClose->setChecked(true);

    connect(ui->radioButtonMcuClose, SIGNAL(clicked()), this, SLOT(GroupMcuUartRadioClick()));
    connect(ui->radioButtonMcuOpen, SIGNAL(clicked()), this, SLOT(GroupMcuUartRadioClick()));
    connect(ui->radioButtonPlcClose, SIGNAL(clicked()), this, SLOT(GroupPlcUartRadioClick()));
    connect(ui->radioButtonPlcOpen, SIGNAL(clicked()), this, SLOT(GroupPlcUartRadioClick()));

}

int dialValueOld = 0;
void DataMenu::on_dial_valueChanged(int value)
{
    if(dialValueOld < value)
    {
        // qDebug()<<"da";
        chart->zoom(1.1);
    }
    else if(dialValueOld > value)
    {
        // qDebug()<<"xiao";
        chart->zoom(10.0/11);
    }
    dialValueOld = value;
}







void DataMenu::timerSlot()
{
    updateDataCharts();
}


void DataMenu::tipSlot(QPointF position, bool isHovering)
{
    // qDebug()<<"tip:"<<tip;
    if (tip == 0)
        tip = new Callout(chart);

    if (isHovering)
    {
        tip->setText(QString("X: %1 \nY: %2 ").arg(position.x()).arg(position.y()));
        tip->setAnchor(position);
        tip->setZValue(11);
        tip->updateGeometry();
        tip->show();
    }
    else
    {
        tip->hide();
    }
}
void DataMenu::refreshData()
{
    if(!(timeCount % 5))
    {
        refreshTime();
        runTime ++;
    }
    timeCount++;
}

void DataMenu::refreshTime()
{
    //current time
    QString time  = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->timeLable->setText(time);
    //system run time
    // ui->runTimeLable->setText(QString::number(runTime) + "秒");
    ui->lcdNumber->display(QString::number(runTime));
}






//void DataMenu::updateDataCharts()
//{

//#if 1
//    int i;
//    QVector<QPointF> oldData = series->pointsVector();
//    QVector<QPointF> data;

//    if (oldData.size() < 360) {
//        data = series->pointsVector();
//    }
//    else
//    {
//        /* 添加之前老的数据到新的vector中，不复制最前的数据，即每次替换前面的数据
//         由于这里每次只添加1个数据，所以为1，使用时根据实际情况修改*/
//        for (i = 1; i < oldData.size(); ++i)
//        {
//            data.append(QPointF(i - 1 , oldData.at(i).y()));
//        }
//    }

//    qint64 size = data.size();
//    /* 这里表示插入新的数据，因为每次只插入1个，这里为i < 1,
//     但为了后面方便插入多个数据，先这样写*/
//    float y ;
//    for(i = 0; i < 1; ++i)
//    {
//        y = 10 * sin(M_PI * count * 4 / 180);
//        data.append(QPointF(i + size, y));
//        chart->setBackgroundPen(QPen(Qt::red));
//        if(y > 0)
//        {
//            //series->setColor(Qt::red);
//            series->setPen((QPen(Qt::red)));
//        }
//        else
//        {
//            // series->setColor(Qt::yellow);
//            series->setPen((QPen(Qt::yellow)));
//        }
//    }
//    ui->lcdNumber_StatusInformation->display(y);
//    ui->lcdNumber_ZeroMagneticField->display(y-2);
//    ui->lcdNumber_ABMagneticField->display(123.456);

//    //      ui->label_value_n_max->setText( QString::number(y,'f',1));


//    series->replace(data);
//    count++;
//#endif
//}


void DataMenu::updateDataCharts()
{
    //     QVector<QPointF> data;
    //     QVector<QPointF> oldData = series->pointsVector();
    // qDebug()<<"olddatasize:"<<oldData.size();

    series1->replace(GloableData::GetInstance()->pointList1);
    series2->replace(GloableData::GetInstance()->pointList2);

    //  timerChart->stop();

}










#define MARK_BUTTON_SLOTS

void DataMenu::on_setBtn_clicked()
{
    disconnect(GloableData::GetInstance()->serial, SIGNAL(readyRead()), this, SLOT(serialPlcReciveMannage()));
    SettingMenu settingMenu;
    settingMenu.exec();
    connect(GloableData::GetInstance()->serial, SIGNAL(readyRead()), this, SLOT(serialPlcReciveMannage()));
}

void DataMenu::on_setZeroBtn_clicked()
{
    qDebug()<<"电机置零";

    modbus_set_slave(GloableData::GetInstance()->mb, 1);
    modbus_write_bit(GloableData::GetInstance()->mb, 0x0102, 1);
    Utils::sleep(50);
    modbus_write_bit(GloableData::GetInstance()->mb, 0x0102, 0);



#if 0
    // McuCommunicateService *mcuServi


    ce = McuCommunicateService::GetInstance();
    // mcuService->addCmd(DataDef::C_MCU_MODBUS_READ_REGISTER);

#ifdef PLC_TEST_SEND

    //    QMap<DataDef::PlcCmdType, DataDef::DataStruct>tempPlcMap =qcmdProcess->getPlcMap();

    //    QMap<DataDef::PlcCmdType, DataDef::DataStruct>::iterator i;
    //    for(i=tempPlcMap.begin();i!=tempPlcMap.end();i++)
    //    {
    //       qDebug()<<i.key()<<"   " << i.value().data<<"  "<<i.value().dataLen;
    //       serial->write(i.value().data);
    //       serial->write("::");
    //    }


#endif

#endif
}
//电机使能BTN
void DataMenu::on_setMotorEnableBtn_clicked()
{

    qDebug()<<"电机使能锁死";

    GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ENABLE_LOCKED).value().data);

#if 0
    //QMap<DataDef::PlcCmdType, DataDef::DataStruct> &tempPlcMap = qcmdProcess->getPlcMap();;
    // serial->write(tempPlcMap.find(DataDef::C_ENABLE_LOCKED).value().data);
    //GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ENABLE_LOCKED).value().data);
#endif
}

void DataMenu::on_cmdStopStartBtn_clicked()
{
    if (QObject::sender() == ui->cmdStopStartBtn)
    {
        //if (!isStopping)
        {
            timerChart->stop();
            //  ui->cmdStopStartBtn->setText("继续采集");
        }

        //   isStopping = !isStopping;
    }
}

void DataMenu::on_cmdContinueBtn_clicked()
{
    if (QObject::sender() == ui->cmdContinueBtn)
    {


        // if (isStopping)
        {
            timerChart->start();
            //  ui->cmdStopStartBtn->setText("停止采集");
        }
        //  isStopping = !isStopping;
    }
}


void DataMenu::on_lineClearBtn_clicked()
{
    lineClear();

    // isStopping = ~isStopping;
}


/*1.press first: 开始采集
 *2.second press： 停止采集
 *3.trd ： 清除波形
*/
int FlagGo = 0;
void DataMenu::on_pushButtonGOGOGO_clicked()
{
    switch (FlagGo)
    {
    case 0:
        FlagGo++;ui->pushButtonGOGOGO->setText("停止采集");

        GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ENABLE_LOCKED).value().data);
        Utils::sleep(200);
        GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ROTATE_A).value().data);
        Utils::sleep(200);
        GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ROTATE_B).value().data);

    //    Utils::sleep(1000);



        timerChart->start();
        McuCommunicateService::GetInstance()->addCmd(DataDef::READ_POINTS);


        break;
    case 1:
        FlagGo++;ui->pushButtonGOGOGO->setText("波形清除"); timerChart->stop();
        break;
    case 2:
        FlagGo=0;ui->pushButtonGOGOGO->setText("开始采集"); DataClean(); lineClear();
        break;

    }


}




void DataMenu::on_pushButtonUsartStart_clicked()
{
    if(flagUartOpen == 1)
    {
        QMessageBox::about(NULL, "报告", QString("串口已打开！"));
        return;
    }


#ifdef ONLY_USE_PLC_UART
    initPlcSerialService();
#else
    initPlcSerialService();
    initMcuSerialService();
#endif

    //qDebug()<<GloableData::GetInstance()->serial->isOpen();
    //qDebug()<<GloableData::GetInstance()->mb;
    if(ui->portCbx_stm->currentText() == ui->portCbx_plc->currentText())
    {
        QMessageBox::about(NULL, "报告", QString("串口打开失败，请重启！"));
        this->close();
        return;
    }
    if(GloableData::GetInstance()->serial->isOpen() == true && NULL !=  GloableData::GetInstance()->mb)
    {

        ui->pushButtonUsartStart->setText("已开启");
        ui->pushButtonUsartStart->setStyleSheet("background-color: rgb(0, 255, 0)");
        QMessageBox::about(NULL, "报告", QString("串口打开成功！"));
        this->flagUartOpen =1;
    }
    return ;
}
void DataMenu::DataClean()
{
    qDebug()<<"button清除数据";
    modbus_set_slave(GloableData::GetInstance()->mb, 1);
    modbus_write_bit(GloableData::GetInstance()->mb, 0x0100, 1);
    Utils::sleep(10);
    modbus_write_bit(GloableData::GetInstance()->mb, 0x0100, 0);


    ui->lcdNumber_StatusInformation->display("");
    ui->lcdNumber_ZeroMagneticField->display("");
    ui->lcdNumber_ABMagneticField->display("");
    GloableData::GetInstance()->pointList1.clear();
    GloableData::GetInstance()->pointList2.clear();
    ui->tableWidget->clearContents();
    ui->tableWidget_2->clearContents();
    initTableWidget();
    initTableWidget_2();
}

void DataMenu::lineClear()
{
    timerChart->stop();
    series1->clear();
    series2->clear();
    GloableData::GetInstance()->pointList1.clear();
    GloableData::GetInstance()->pointList2.clear();


    modbus_set_slave(GloableData::GetInstance()->mb, 1);
    modbus_write_bit(GloableData::GetInstance()->mb, 0x0100, 1);
    Utils::sleep(10);
    modbus_write_bit(GloableData::GetInstance()->mb, 0x0100, 0);
}




void DataMenu::on_pushButtonDataClean_clicked()
{
    DataClean();
}

void DataMenu::on_pushButtonMagneticFieldToZero_clicked()
{
    qDebug()<<"button磁场回零";

    //uint16_t temp_buff[10]={1,2,3,4,5,6,7,8,9,10};
    modbus_set_slave(GloableData::GetInstance()->mb, 1);
    //qDebug()<<"ret:"<<  modbus_write_registers(GloableData::GetInstance()->mb, 0x101, i, temp_buff);

    modbus_write_bit(GloableData::GetInstance()->mb, 0x0101, 1);
    Utils::sleep(20);
    modbus_write_bit(GloableData::GetInstance()->mb, 0x0101, 0);
}

void DataMenu::on_pushButtonMotorCheck_clicked()
{
    qDebug()<<"button电机检测";
}

void DataMenu::on_pushButtonWarningZero_clicked()
{
    qDebug()<<"button报警清零";
}


void DataMenu::GroupMcuUartRadioClick()
{
    switch(btnGroupUartMcu->checkedId())
    {
    case RADIOBUTTON_CLOSE_MCU:


        modbus_close(GloableData::GetInstance()->mb);
        //  modbus_free(GloableData::GetInstance()->mb);
        flagUartOpenMcu =0;
        QMessageBox::about(NULL, "报告", QString("MCU串口已关闭！"));

        break;
    case RADIOBUTTON_OPEN_MCU:
        if(0 == flagUartOpenMcu)
        {

            initMcuSerialService();
            if(NULL !=  GloableData::GetInstance()->mb)
            {
                QMessageBox::about(NULL, "报告", QString("MCU串口打开成功！"));
                flagUartOpenMcu = 1;

                timer_updatastatue->start(50);
            }
        }
        else
        {
            QMessageBox::about(NULL, "报告", QString("MCU串口已经打开！"));
        }
        break;
    }
}

void DataMenu::GroupPlcUartRadioClick()
{
    switch(btnGroupUartPlc->checkedId())
    {
    case RADIOBUTTON_CLOSE_PLC:

        GloableData::GetInstance()->serial->close();
        flagUartOpenPlc = 0;
        QMessageBox::about(NULL, "报告", QString("PLC串口已关闭！"));
        break;
    case RADIOBUTTON_OPEN_PLC:

        if(0==flagUartOpenPlc)
        {

            initPlcSerialService();
            if( true == GloableData::GetInstance()->serial->isOpen())
            {
                QMessageBox::about(NULL, "报告", QString("PLC串口打开成功！"));
                flagUartOpenPlc = 1;
            }
        }
        else
        {
            QMessageBox::about(NULL, "报告", QString("PLC串口已经打开！"));
        }
        break;
    }
}
void execl_tab_out1(void)
{
    ExcelEngine excel(QObject::tr("./../Desktop/MonitorReport_1.xls")); //创建
    excel.Open(); //打开

    excel.SaveDataFrTable(GloableData::GetInstance()->tableWidget);

    //    excel.mergeCells("A5:C7");
    //    excel.SetCellData(5,1,"666666");
    //     excel.setCellTextCenter(5,1);
    //     excel.setCellTextColor(5,1);
    excel.Save(); //保存
    excel.Close();
    qDebug()<<"file output OK!!";

}
void execl_tab_out2(void)
{
    ExcelEngine excel(QObject::tr("./../Desktop/MonitorReport_2.xls")); //创建
    excel.Open(); //打开
    excel.SaveDataFrTable(GloableData::GetInstance()->tableWidget_2);
    //    excel.mergeCells("A5:C7");
    //    excel.SetCellData(5,1,"666666");
    //     excel.setCellTextCenter(5,1);
    //     excel.setCellTextColor(5,1);
    excel.Save(); //保存
    excel.Close();
    qDebug()<<"file output OK!!";

}
void DataMenu::on_setBtnOutPutData_clicked()
{
    execl_tab_out1();
    execl_tab_out2();
    QMessageBox::about(NULL, "报告", QString("报表已发送至桌面"));
}

int Flag_TabWidget_Show = 0;
void DataMenu::on_pushButtonChangeTabWidget_clicked()
{
    if(0==Flag_TabWidget_Show)
    {
        ui->tableWidget->show();
        ui->tableWidget_2->hide();
        Flag_TabWidget_Show = 1;
        return;
    }
    if(1==Flag_TabWidget_Show)
    {

        ui->tableWidget->hide();
        ui->tableWidget_2->show();

        Flag_TabWidget_Show = 0;
        return;
    }
    return;
}
void DataMenu::on_lineEditLineNumChange_textChanged(const QString &arg1)
{
    if( 1!= ui->tableWidget_2->isHidden())
    {
        ui->tableWidget_2->verticalScrollBar()->setSliderPosition(ui->lineEditLineNumChange->text().toInt()-1);
    }
    else{
        ui->tableWidget->verticalScrollBar()->setSliderPosition(ui->lineEditLineNumChange->text().toInt()-1);
    }
}


#define LJH_LOG_OUT_TXT(contentString) Utils::writeLogTXT(contentString,__FILE__, __LINE__,__FUNCTION__);

void DataMenu::TEST0()
{
    McuCommunicateService *mcuService = McuCommunicateService::GetInstance();
    mcuService->addCmd(DataDef::READ_POINTS);

}
void DataMenu::TEST1()
{
    //  GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ENABLE_LOCKED).value().data);


    uint16_t temp_buff[3600]={0};




    //     modbus_set_slave(GloableData::GetInstance()->mb, 1);
    //     modbus_read_registers( GloableData::GetInstance()->mb, 4096, 100, temp_buff);

    //     for(int i = 0;i<100;i++)
    //          {
    //              qDebug()<<temp_buff[i];
    //          }

    modbus_set_slave(GloableData::GetInstance()->mb, 2);

    for(int s = 0;s<36;s++)
    {

        modbus_read_registers( GloableData::GetInstance()->mb, s*100, 100, &temp_buff[s*100]);
    }

    for(int i = 0;i<3600;i++)
    {
        qDebug()<<temp_buff[i];
    }


    // modbus_read_registers( GloableData::GetInstance()->mb, 0, 100, temp_buff);

}
void DataMenu::TEST2()
{

    //   ui->lcdNumber_StatusInformation
    uint16_t temp_buff[10]={1,2,3,4,5,6,7,8,9,10};
    modbus_set_slave(GloableData::GetInstance()->mb, 1);
    qDebug()<<"ret:"<<  modbus_write_registers(GloableData::GetInstance()->mb, 8192, 10, temp_buff);


#if 0
    //    int temp = 1;
    uint16_t temp_buff[100]={0};
    //    temp_buff[0] = 0;
    for(int i = 0;i<100;i++)
    {
        if(i<50)
        {
            temp_buff[i] = i*10;
        }
        else
        {
            temp_buff[i] = -i*10;
        }
    }


    //     modbus_set_slave(GloableData::GetInstance()->mb, 1);
    //     modbus_read_registers( GloableData::GetInstance()->mb, 4096, 100, temp_buff);

    //     for(int i = 0;i<100;i++)
    //          {
    //              qDebug()<<temp_buff[i];
    //          }

    modbus_set_slave(GloableData::GetInstance()->mb, 2);

    for(int s = 0;s<36;s++)
    {
        //        temp_buff[99] = temp;
        //        temp++;
        // modbus_write_registers(GloableData::GetInstance()->mb, s*100, 100, temp_buff);
        modbus_write_registers(GloableData::GetInstance()->mb, s*100, 100, temp_buff);
    }


    // modbus_read_registers( GloableData::GetInstance()->mb, 0, 100, temp_buff);




#endif

}

void DataMenu::updatastau()
{
    qDebug() << "in updatastau";
    uint16_t temp_buff[32]={0};
    // QMap<int,ModusBuffDef::RegisterDataStruct> PointBuffMap;  //key 考虑用x轴角度值,value为 当前角度 对应的
    //  ModusBuffDef::RegisterDataStruct data;
    // memset(&data,0,sizeof(data));
    modbus_set_slave(GloableData::GetInstance()->mb, 1);
    // for(int cnt_num=0;cnt_num<1;cnt_num++)
    // {
    int regs=modbus_read_registers(GloableData::GetInstance()->mb, 4096, 10, temp_buff);
    //  memcpy(&data,temp_buff,10*sizeof(uint16_t));
    //  PointBuffMap.insert(0, data);
    // }


    //   for(int i= 0;i<11;i++)
    //   {
    //        qDebug()<<temp_buff[i];

    //   }


    //flush 实时AB磁场
    if(QString::number(temp_buff[7], 10).toFloat()>32767)
    {
        ui->lcdNumber_ABMagneticField->display(    (QString::number(temp_buff[7], 10).toFloat()-65536)  /10       );
    }
    else
    {
        ui->lcdNumber_ABMagneticField->display( (QString::number(temp_buff[7], 10).toFloat())/10);
    }


    //flush 实时零位磁场
    if(QString::number(temp_buff[8], 10).toFloat()>32767)
    {
        ui->lcdNumber_ZeroMagneticField->display( (QString::number(temp_buff[8], 10).toFloat()-65536)/10);
    }
    else
    {
        ui->lcdNumber_ZeroMagneticField->display( (QString::number(temp_buff[8], 10).toFloat())/10);
    }


    //    /**************************************************************************************************************************************/
    //           //磁极数
    //            if(QString::number(PointBuffMap.find(0).value().num_magnetic_pole, 10).toFloat()>32767)
    //            {
    //               ui->tableWidget->setItem(2,1,new QTableWidgetItem( QString::number   (QString::number(PointBuffMap.find(0).value().num_magnetic_pole, 10).toFloat()-65536),10 ));
    //            }
    //          else
    //            {
    //             ui->tableWidget->setItem(2,1,new QTableWidgetItem( QString::number   (QString::number(PointBuffMap.find(0).value().num_magnetic_pole, 10).toFloat()),10 ));
    //            }

    //          //N极最大磁场
    //            if(QString::number(PointBuffMap.find(0).value().value_n_max, 10).toFloat()>32767)
    //            {
    //               ui->tableWidget->setItem(2,2,new QTableWidgetItem( QString::number   ((QString::number(PointBuffMap.find(0).value().value_n_max, 10).toFloat()-65536)/10),10 ));
    //            }
    //          else
    //            {
    //             ui->tableWidget->setItem(2,2,new QTableWidgetItem( QString::number   ((QString::number(PointBuffMap.find(0).value().value_n_max, 10).toFloat())/10),10 ));
    //            }

    //            //S极最大磁场
    //              if(QString::number(PointBuffMap.find(0).value().value_s_max, 10).toFloat()>32767)
    //              {
    //                 ui->tableWidget->setItem(2,3,new QTableWidgetItem( QString::number   ((QString::number(PointBuffMap.find(0).value().value_s_max, 10).toFloat()-65536)/10),10 ));
    //              }
    //            else
    //              {
    //               ui->tableWidget->setItem(2,3,new QTableWidgetItem( QString::number   ((QString::number(PointBuffMap.find(0).value().value_s_max, 10).toFloat())/10),10 ));
    //              }

    //              //N极最小磁场
    //                if(QString::number(PointBuffMap.find(0).value().value_n_min, 10).toFloat()>32767)
    //                {
    //                   ui->tableWidget->setItem(2,4,new QTableWidgetItem( QString::number   ((QString::number(PointBuffMap.find(0).value().value_n_min, 10).toFloat()-65536)/10),10 ));
    //                }
    //              else
    //                {
    //                 ui->tableWidget->setItem(2,4,new QTableWidgetItem( QString::number   ((QString::number(PointBuffMap.find(0).value().value_n_min, 10).toFloat())/10),10 ));
    //                }

    //                //S极最小磁场
    //                  if(QString::number(PointBuffMap.find(0).value().value_s_min, 10).toFloat()>32767)
    //                  {
    //                     ui->tableWidget->setItem(2,5,new QTableWidgetItem( QString::number   ((QString::number(PointBuffMap.find(0).value().value_s_min, 10).toFloat()-65536)/10),10 ));
    //                  }
    //                else
    //                  {
    //                   ui->tableWidget->setItem(2,5,new QTableWidgetItem( QString::number   ((QString::number(PointBuffMap.find(0).value().value_s_min, 10).toFloat())/10),10 ));
    //                  }



#if 0
    uint16_t temp_buff[2]={0};
    modbus_set_slave(GloableData::GetInstance()->mb, 2);


    modbus_read_registers( GloableData::GetInstance()->mb, 0, 2, temp_buff);

    //QString::number(temp_buff[s*100 + i], 10).toFloat()
    for(int i = 0;i<2;i++)
    {
        qDebug()<<"temp_buff[i]:"<<temp_buff[i];
        if(QString::number(temp_buff[i], 10).toFloat()>32767)
        {
            qDebug()<<"!!-32768 temp_buff[i]QString::number:"<<QString::number(temp_buff[i], 10).toFloat()-65536;

        }
        else
        {
            qDebug()<<"temp_buff[i]QString::number:"<<QString::number(temp_buff[i], 10).toFloat();
        }
    }
#endif
}

void DataMenu::updatastau2()
{
    McuCommunicateService::GetInstance()->addCmd(DataDef::READ_STATUS);
    ui->lcdNumber_ABMagneticField->display(GloableData::GetInstance()->realTimeAB);
    ui->lcdNumber_ZeroMagneticField->display(GloableData::GetInstance()->realTimeZero);
}


/*

我转换后的10进制数据是否大于32767，如果大于，则让它减去65536，否则不变。这样就能得到正确的有符号数据了。
*/
void DataMenu::TEST3()
{

    updatastau();

}

void DataMenu::TEST4()
{
    uint16_t temp_buff[1]={0};
    modbus_set_slave(GloableData::GetInstance()->mb, 1);

    modbus_read_registers( GloableData::GetInstance()->mb,0x1010, 1, temp_buff);


    qDebug()<<temp_buff[1];
}

void DataMenu::TEST5()
{

    uint16_t temp_buff[1]={0};
    modbus_set_slave(GloableData::GetInstance()->mb, 1);

    modbus_read_registers( GloableData::GetInstance()->mb,4096+10, 1, temp_buff);
    //int regs=modbus_read_registers(GloableData::GetInstance()->mb, 4096, 11, temp_buff);
    qDebug()<<temp_buff[0];
}


void DataMenu::on_pushButtonGoTest_clicked()
{
    // qDebug()<<ui->comboBoxFuncTest->currentText().toLatin1().data();
    //qDebug()<<ui->comboBoxFuncTest->currentIndex();
    switch (ui->comboBoxFuncTest->currentIndex())
    {
    case 0 :   TEST0(); break;
    case 1 :   TEST1(); break;
    case 2 :   TEST2(); break;
    case 3 :   TEST3(); break;
    case 4 :   TEST4(); break;
    case 5 :   TEST5(); break;


    default:   break;

    }
}







void DataMenu::on_horizontalSlider_valueChanged(int value)
{
    if(value<100)
        chart->axisX()->setRange(0, 360-3.6*value);
}

void DataMenu::on_verticalSlider_valueChanged(int value)
{
    if(value<80)
        chart->axisY()->setRange(-80, 80-value);
}



void DataMenu::on_pushButtonResetvaluesc_clicked()
{
    chart->axisX()->setRange(0, 360);
    chart->axisY()->setRange(-80, 80);
}
