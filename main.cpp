#include "mainwindow.h"
#include <QApplication>
#include  "datamenu.h"
#include "uartsetting.h"
#include <ModbusService/mcucommunicateservice.h>
#include <ModbusService/plccommunicateservice.h>
#include "GloableData/gloabledata.h"
#include <macro_define.h>
#include <QTextStream>
#include <QByteArray>
#include <QDebug>
#include <QString>
#include "execltool/qexcel.h"
#include <QTableWidget>
#include "qtablewidget.h"
#include <QWidget>
//#include <QtGui/QApplication>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QSplashScreen>
#include "databasetool/databaseengine.h"


void dataBaseInit(void)
{

    MYDataBase *d = new MYDataBase();
    d->createConnection(".//PointData1.db");  //创建连接
    d->createTable("create table point_table1(point_id int,x double,y double,info text)");
    // d.insert(2,0.345,988.345,"test23");
    GloableData::GetInstance()->g_database = d;
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef UART_FIRST_SETTING_SHOW
    //UartSetting uartSetting;
    //uartSetting.show();
    DataMenu dataMenu;

#else
    MainWindow mainWindow;
    mainWindow.show();
#endif
    QSplashScreen *splash=new QSplashScreen;
    QPixmap pix(":/new/img/resources/start_logo.png");
    splash->showMessage("正在载入，请稍等……");
    splash->setPixmap(pix);
    splash->show();
    a.processEvents();//这样可以保证程序一直可以响应鼠标键盘上事件
  //  Utils::sleep(3000);
    splash->finish(&dataMenu);//启动画面在窗口w创建完成以后结束
    dataMenu.show();

          QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

       dataBaseInit();





         //  d.queryAll();          //已经完成过createTable(), insert(), 现在进行查询
          //  d.deleteById(7);

         //  d.updateById(5);
            //d.queryAll();




    return a.exec();
}
