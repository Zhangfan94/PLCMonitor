#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "libmodbus/modbus.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rtuBtn_clicked()
{

    //RTU
    modbus_t *mb;
    uint16_t tab_reg[32]={0};

    mb = modbus_new_rtu("COM3", 115200, 'E', 8, 1);   //相同的端口只能同时打开一个

    modbus_set_slave(mb, 1);  //设置modbus从机地址
    modbus_connect(mb);

    struct timeval t;
    t.tv_sec=0;
    t.tv_usec=1000000;   //设置modbus超时时间为1000毫秒
    modbus_set_response_timeout(mb, &t);

    int regs=modbus_read_registers(mb, 0x1000, 10, tab_reg);
    qDebug() << "regs = " << regs;
    qDebug("%x", tab_reg[7]);
//    for(int i = 0; i < 20; i++)
//    {
//        //std::cout << tab_reg[i];
//        qDebug("%x", tab_reg[i]);
//    }
//    QMessageBox::about(NULL, "报告", QString("RTU 读取寄存器的个数: %1").arg(regs));

    int ret = modbus_write_bit(mb, 0x101, 1);
    qDebug() << ret;
    ret = modbus_write_bit(mb, 0x101, 0);
    qDebug() << ret;

    regs = modbus_read_registers(mb, 0x1000, 10, tab_reg);
    qDebug() << "regs = " << regs;
    qDebug("%x", tab_reg[7]);

//    for(int i = 0; i < 20; i++)
//    {
//        //std::cout << tab_reg[i];
//        qDebug("%x", tab_reg[i]);
//    }
    modbus_close(mb);
    modbus_free(mb);
}

void MainWindow::on_tcpBtn_clicked()
{
    //TCP
    modbus_t *mb;

    uint16_t tab_reg[32]={0};
    mb = modbus_new_tcp("127.0.0.1", 502);  //由于是tcp client连接，在同一个程序中相同的端口可以连接多次。

    modbus_set_slave(mb, 1);  //从机地址
    modbus_connect(mb);

    struct timeval t;
    t.tv_sec=0;
    t.tv_usec=1000000;   //设置modbus超时时间为1000毫秒，注意：经测试，如果没有成功建立tcp连接，则该设置无效。
    modbus_set_response_timeout(mb, &t);

    int regs=modbus_read_registers(mb, 0, 10, tab_reg);

    QMessageBox::about(NULL, "报告", QString("TCP 读取寄存器的个数: %1").arg(regs));
    modbus_close(mb);
    modbus_free(mb);
}
