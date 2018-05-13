#include "settingmenu.h"
#include "ui_settingmenu.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <Qdebug>
#include <QTime>
#include <QMessageBox>
#include <GloableData/gloabledata.h>
#include "utils.h"
#include <iostream>


SettingMenu::SettingMenu(QWidget *parent) :
    QDialog(parent),
    Flag_Get_Para(0),
    ui(new Ui::SettingMenu)
{
    ui->setupUi(this);

    mainByteArray.clear();
    serial = GloableData::GetInstance()->serial;
    serialCheck();

    connect(serial, SIGNAL(readyRead()), this, SLOT(settingMenuPlcRead()));
    connect(&timerGetPara, SIGNAL(timeout()), this, SLOT(requestByTimer()));

    initProgressBar();

     this->setWindowTitle("设置");
}

SettingMenu::~SettingMenu()
{
    delete ui;
}

void SettingMenu::serialCheck()
{
    if((NULL != serial ) &&( true == serial->isOpen()))
    {
        ui->textEdit_2->append("串口接收监听中..." );
    }
    else
    {
        ui->textEdit_2->append("请打开串口再试！" );
        QMessageBox::about(NULL, "报告", QString("串口未开启！"));
    }

}

void SettingMenu::initProgressBar()
{
    ui->progressBar->setOrientation(Qt::Horizontal);  // 水平方向
    ui->progressBar->setMinimum(0);  // 最小值
    ui->progressBar->setMaximum(100);  // 最大值
    ui->progressBar->setValue(0);  // 当前进度
}



void SettingMenu::showEvent(QShowEvent *event)
{
    // timerGetPara.start(1000);
    timerGetPara.start(200);
}


/*
    uint16_t temp_buff[32]={0};


     modbus_set_slave(GloableData::GetInstance()->mb, 1);

        int regs=modbus_read_registers(GloableData::GetInstance()->mb, 4096, 10, temp_buff);




            //flush 实时AB磁场
            if(QString::number(PointBuffMap.find(0).value().value_cur_AB, 10).toFloat()>32767)
            {
               ui->lcdNumber_ABMagneticField->display(    (QString::number(PointBuffMap.find(0).value().value_cur_AB, 10).toFloat()-65536)  /10       );
            }
*/
void SettingMenu::requestByTimer()
{
    Flag_Get_Para++;
    qDebug()<<Flag_Get_Para;
    ui->progressBar->setValue(Flag_Get_Para*10);  // 当前进度

    uint16_t temp_buff[32]={0};
    modbus_set_slave(GloableData::GetInstance()->mb, 1);
    switch(Flag_Get_Para)
    {
    case 1:
        GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_READ_SPEED).value().data);
        break;
    case 2:
        GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_READ_ANGLE).value().data);
        break;
    case 3:
        modbus_read_registers(GloableData::GetInstance()->mb, 8192, 10, temp_buff);
        for(int i =0;i<10;i++)
        {

            qDebug()<<"tempbuff:"<<temp_buff[i];

        }


        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;

    default:
        break;
    }

    if(Flag_Get_Para>=10)
    {
        Flag_Get_Para=0;
        timerGetPara.stop();
    }
}


void SettingMenu::on_retBtn_clicked()
{
    //this->close();
    disconnect(serial, SIGNAL(readyRead()), this, SLOT(settingMenuPlcRead()));
    reject();
}


void SettingMenu::on_pushButton_Test_clicked()
{
    //协议 字节 必须小于 127   ，这里只能传char  -128  ——————   + 127
    //char data[]={0x11,0x22,0x11,0x62,0x11,0x22,0x61,0x22,0x11,0x62,0x11,0x22,0x11,0x1d,0x11,0x22,0x11,0x22};
    //serial->write(data,sizeof(data)/sizeof(unsigned char));
    //serial->write(ui->textEdit->toPlainText().toLocal8Bit().data());
}


void SettingMenu::textEditShow( QByteArray ba)
{
    ui->textEdit_2->setTextColor(QColor(Qt::red));
    ui->textEdit_2->append("(hex)"+(QString)ba.toHex());
    ui->textEdit_2->append("(ascii)"+(QString)ba);
    ui->textEdit_2->setTextColor(QColor(Qt::green));
    ui->textEdit_2->append("---------------------------"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") );
}



void SettingMenu::settingMenuPlcRead()
{
    QByteArray bArryIn;
    bool ok;
    int i;
#if 1
    QByteArray temp = serial->readAll();
    //这里 需要处理一下 ！！！！单次 readall接受不全的问题，需要分次接收，约定 一个结束位 #
    if(!temp.isEmpty())
    {
        mainByteArray.append(temp);
        if(mainByteArray.contains(0x03))
        {
            textEditShow(mainByteArray);

            switch(Flag_Get_Para)
            {
            case 1:
                //qDebug()<<"setting recive 1:"<< mainByteArray.data();
                for(i = 6;i<=13;i++)
                {
                     bArryIn.append(mainByteArray.at(i));
                }
                //            QString str = QString(xx);
                //            int hex = str.toInt(&ok, 16);       // hex == 255, ok == true
                //            qDebug()<<hex;
                //            qDebug()<<ok;
                //           qDebug()<< QString::number(hex,10);

                ui->lineEdit_ValueMotorSpeed->setText(   QString::number(       QString(bArryIn).toInt(&ok,16),10)        );
                break;

            case 2:
               // qDebug()<<"setting recive 2:"<< mainByteArray.data();
                for(i = 6;i<=13;i++)
                {
                     bArryIn.append(mainByteArray.at(i));
                }
                ui->lineEdit_ValueMotorAngle->setText(   QString::number(       QString(bArryIn).toInt(&ok,16),10)        );
                break;

            case 3:
                break;

            default :
                break;

            }
            mainByteArray.clear();
        }
    }
#endif
}





void SettingMenu::sendArrayConfig(char* array,int array_length,QString linedit_hex_get)
{
    char str_hex_put[5];
    for(int i = 0;i<5;i++)
    {
        str_hex_put[i] = linedit_hex_get.toUpper().toLatin1().data()[i]; //这里存在小写 转大写问题
    }
    memcpy(&array[22-linedit_hex_get.size()],str_hex_put, linedit_hex_get.size());
    unsigned char de = Utils::lrcCaculate( array,array_length);
    array[22] = Utils::heToAsciiNum(de>>4) ;
    array[23] = Utils::heToAsciiNum(de&0x0f) ;
}
//应用电机参数
void SettingMenu::on_pushButtonMotorParaApply_clicked()
{
    //set Angle value                01        47          01          D    R    0    0    0    0   2
    char cmdDataSetAngle[]={0x02, 0x30,0x31, 0x34,0x37, 0x30,0x31,    0x44,0x52,0x30,0x30,0x30,0x30,0x32,   0x30,0x30, 0x30,0x30,   0x30,0x30, 0x30,0x30,   0x00,0x00,   0x03}; //设置 角度
    //set Speed value                  01        47         01         D    R    0    0   0    0    0
    char cmdDataSetSpeed[]={0x02, 0x30,0x31, 0x34,0x37, 0x30,0x31,    0x44,0x52,0x30,0x30,0x30,0x30,0x30,   0x30,0x30, 0x30,0x30,   0x30,0x30, 0x30,0x30,   0x00,0x00,   0x03}; //设置 转速

    QString str_hex_lineEdit_ValueMotorAngle =  QString::number(ui->lineEdit_ValueMotorAngle->text().trimmed().toInt(), 16);//拿到的数据 转为16进制 的 QString
    QString str2_hex_lineEdit_ValueMotorSpeed = QString::number(ui->lineEdit_ValueMotorSpeed->text().trimmed().toInt(), 16);//拿到的数据 转为16进制 的 QString

    sendArrayConfig(cmdDataSetAngle,sizeof(cmdDataSetAngle),str_hex_lineEdit_ValueMotorAngle);
    sendArrayConfig(cmdDataSetSpeed,sizeof(cmdDataSetSpeed),str2_hex_lineEdit_ValueMotorSpeed);

    serial->write(cmdDataSetAngle,sizeof(cmdDataSetAngle)/sizeof(char));
    Utils::sleep(200);
    serial->write(cmdDataSetSpeed,sizeof(cmdDataSetSpeed)/sizeof(char));
}



void SettingMenu::on_pushButtonMcuCollectionParaApply_clicked()
{


}



void SettingMenu::on_pushButtonMotorTest_clicked()
{
    serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ENABLE_LOCKED).value().data);
    Utils::sleep(200);
    serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ROTATE_A).value().data);
    Utils::sleep(200);
    serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ROTATE_B).value().data);
}

static void TEST0(void)
{
    qDebug()<<"TEST0 停止";
    GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_STOP).value().data);

}
static void TEST1(void)
{

    qDebug()<<"TEST1 一直转";
    GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ROTATE_ALL_TIME).value().data);
}
static void TEST2(void)
{

    qDebug()<<"TEST2 读角度";
    GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_READ_ANGLE).value().data);

}
static void TEST3(void)
{

    qDebug()<<"TEST3 读转速";
    GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_READ_SPEED).value().data);
}
static void TEST4(void)
{

    qDebug()<<"TEST4 读 是否使能";
    GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_READ_STATE_ENABLE).value().data);
}
static void TEST5(void)
{

    qDebug()<<"TEST5 读 是否旋转";
    GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_READ_STATE_ROTATE).value().data);
}
static void TEST6(void)
{

    qDebug()<<"TEST6 使能锁死";
    GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ENABLE_LOCKED).value().data);
}
void SettingMenu::TEST7(void)
{
     qDebug()<<"TEST5 使能放开";
     GloableData::GetInstance()->serial->write( CmdProcess::GetInstance()->getPlcMap().find(DataDef::C_ENABLE_RELEASE).value().data);


}

void SettingMenu::on_pushButtonTest_clicked()
{
    switch (ui->comboBoxTest->currentIndex())
    {
    case 0 :   TEST0(); break;
    case 1 :   TEST1(); break;
    case 2 :   TEST2(); break;
    case 3 :   TEST3(); break;
    case 4 :   TEST4(); break;
    case 5 :   TEST5(); break;
    case 6 :   TEST6(); break;
    case 7 :   TEST7(); break;
    default:   break;

    }
}



void SettingMenu::on_pushButtonMotorParaApply_2_clicked()
{

#if 1
    {
        /*
                            01        47       01      D  R  0  0  0  0   2
           设置转9度   02   30 31    34 37     30 31    44 52 30 30 30 30 32     30 30 30 30 32 33 32 38            34 36 03
                                                                      8个字节  (00002328 -> 9000 -> 9.000)


                            01          47       01      D  R  0  0  0  0  0
     设置速度 18度/s    02   30 31     34 37      30 31    44 52 30 30 30 30 30   30 30 30 30 34 36 35 30           34 34 03
                                                                      8个字节 (00004650 -> 18000 -> 18.000/s)


      02 30 31 34 37 30 31 44 52 30 30 30 30 32 30 30 30 30 32 33 32 38 34 36 03
      02 30 31 34 37 30 31 44 52 30 30 30 30 30 30 30 30 30 34 36 35 30 34 34 03


                */

        //set Angle value                01        47          01          D    R    0    0    0    0   2
        char cmdDataSetAngle[]={0x02, 0x30,0x31, 0x34,0x37, 0x30,0x31,    0x44,0x52,0x30,0x30,0x30,0x30,0x32,   0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,   0x00,0x00,   0x03}; //设置 角度


        //set Speed value                  01        47         01         D    R    0    0   0    0    0
        char cmdDataSetSpeed[]={0x02, 0x30,0x31, 0x34,0x37, 0x30,0x31,    0x44,0x52,0x30,0x30,0x30,0x30,0x30,   0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,   0x00,0x00,   0x03}; //设置 转速

        QString str_hex_lineEdit_ValueMotorAngle = QString::number(ui->lineEdit_ValueMotorAngle->text().trimmed().toInt(), 16);//拿到的数据 转为16进制 的 QString

        QString str2_hex_lineEdit_ValueMotorSpeed = QString::number(ui->lineEdit_ValueMotorSpeed->text().trimmed().toInt(), 16);//拿到的数据 转为16进制 的 QString

        char str_hex_put[5];
        str_hex_put[0] =  str_hex_lineEdit_ValueMotorAngle.toUpper().toLatin1().data()[0];
        str_hex_put[1] =  str_hex_lineEdit_ValueMotorAngle.toUpper().toLatin1().data()[1];
        str_hex_put[2] =  str_hex_lineEdit_ValueMotorAngle.toUpper().toLatin1().data()[2];
        str_hex_put[3] =  str_hex_lineEdit_ValueMotorAngle.toUpper().toLatin1().data()[3];
        str_hex_put[4] =  str_hex_lineEdit_ValueMotorAngle.toUpper().toLatin1().data()[4];


        char str_hex_put_Speed[5];
        str_hex_put_Speed[0] =  str2_hex_lineEdit_ValueMotorSpeed.toUpper().toLatin1().data()[0];
        str_hex_put_Speed[1] =  str2_hex_lineEdit_ValueMotorSpeed.toUpper().toLatin1().data()[1];
        str_hex_put_Speed[2] =  str2_hex_lineEdit_ValueMotorSpeed.toUpper().toLatin1().data()[2];
        str_hex_put_Speed[3] =  str2_hex_lineEdit_ValueMotorSpeed.toUpper().toLatin1().data()[3];
        str_hex_put_Speed[4] =  str2_hex_lineEdit_ValueMotorSpeed.toUpper().toLatin1().data()[4];


        switch (str_hex_lineEdit_ValueMotorAngle.size())
        {
        case 2: cmdDataSetAngle[14] = 0x30;cmdDataSetAngle[15] = 0x30;cmdDataSetAngle[16] = 0x30;cmdDataSetAngle[17] = 0x30;
            cmdDataSetAngle[18] = 0x30;cmdDataSetAngle[19] = 0x30;cmdDataSetAngle[20] = str_hex_put[0];cmdDataSetAngle[21] =str_hex_put[1];
            break;
        case 3: cmdDataSetAngle[14] = 0x30;cmdDataSetAngle[15] = 0x30;cmdDataSetAngle[16] = 0x30;cmdDataSetAngle[17] = 0x30;
            cmdDataSetAngle[18] = 0x30;cmdDataSetAngle[19] = str_hex_put[0];cmdDataSetAngle[20] =str_hex_put[1];cmdDataSetAngle[21] =str_hex_put[2];
            break;
        case 4: cmdDataSetAngle[14] = 0x30;cmdDataSetAngle[15] = 0x30;cmdDataSetAngle[16] = 0x30;cmdDataSetAngle[17] = 0x30;
            cmdDataSetAngle[18] = str_hex_put[0];cmdDataSetAngle[19] =str_hex_put[1];cmdDataSetAngle[20] =str_hex_put[2];cmdDataSetAngle[21] =str_hex_put[3];
            break;
        case 5: cmdDataSetAngle[14] = 0x30;cmdDataSetAngle[15] = 0x30;cmdDataSetAngle[16] = 0x30;cmdDataSetAngle[17] = str_hex_put[0];
            cmdDataSetAngle[18] = str_hex_put[1];cmdDataSetAngle[19] =str_hex_put[2];cmdDataSetAngle[20] = str_hex_put[3];cmdDataSetAngle[21] =str_hex_put[4];
            break;
        }

        //          memcpy(&cmdDataSetAngle[14],str_hex_put,sizeof(str_hex_put));
        //          qDebug()<<sizeof(str_hex_put);
        //           qDebug()<<strlen(str_hex_put);
        //           qDebug()<<cmdDataSetAngle;

        switch (str2_hex_lineEdit_ValueMotorSpeed.size())
        {
        case 2:
            cmdDataSetSpeed[14] = 0x30;cmdDataSetSpeed[15] = 0x30;cmdDataSetSpeed[16] = 0x30;cmdDataSetSpeed[17] = 0x30;
            cmdDataSetSpeed[18] = 0x30;cmdDataSetSpeed[19] = 0x30;cmdDataSetSpeed[20] = str_hex_put_Speed[0];cmdDataSetSpeed[21] =str_hex_put_Speed[1];
            break;
        case 3:
            cmdDataSetSpeed[14] = 0x30;cmdDataSetSpeed[15] = 0x30;cmdDataSetSpeed[16] = 0x30;cmdDataSetSpeed[17] = 0x30;
            cmdDataSetSpeed[18] = 0x30;cmdDataSetSpeed[19] = str_hex_put_Speed[0];cmdDataSetSpeed[20] =str_hex_put_Speed[1];cmdDataSetSpeed[21] =str_hex_put_Speed[2];
            break;
        case 4:
            cmdDataSetSpeed[14] = 0x30;cmdDataSetSpeed[15] = 0x30;cmdDataSetSpeed[16] = 0x30;cmdDataSetSpeed[17] = 0x30;
            cmdDataSetSpeed[18] = str_hex_put_Speed[0];cmdDataSetSpeed[19] =str_hex_put_Speed[1];cmdDataSetSpeed[20] =str_hex_put_Speed[2];cmdDataSetSpeed[21] =str_hex_put_Speed[3];
            break;
        case 5:
            cmdDataSetSpeed[14] = 0x30;cmdDataSetSpeed[15] = 0x30;cmdDataSetSpeed[16] = 0x30;cmdDataSetSpeed[17] = str_hex_put_Speed[0];
            cmdDataSetSpeed[18] = str_hex_put_Speed[1];cmdDataSetSpeed[19] =str_hex_put_Speed[2];cmdDataSetSpeed[20] = str_hex_put_Speed[3];cmdDataSetSpeed[21] =str_hex_put_Speed[4];
            break;
        }


        unsigned char de = Utils::lrcCaculate( cmdDataSetAngle,sizeof(cmdDataSetAngle));
        cmdDataSetAngle[22] = Utils::heToAsciiNum(de>>4) ;
        cmdDataSetAngle[23] =Utils::heToAsciiNum(de&0x0f) ;

        unsigned char de2 = Utils::lrcCaculate( cmdDataSetSpeed,sizeof(cmdDataSetSpeed));
        cmdDataSetSpeed[22] = Utils::heToAsciiNum(de2>>4) ;
        cmdDataSetSpeed[23] = Utils::heToAsciiNum(de2&0x0f) ;

        serial->write(cmdDataSetAngle,sizeof(cmdDataSetAngle)/sizeof(char));

        Utils::sleep(200);

        serial->write(cmdDataSetSpeed,sizeof(cmdDataSetSpeed)/sizeof(char));

    }
#endif

}
