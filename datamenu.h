#ifndef DATAMENU_H
#define DATAMENU_H

#include <QWidget>
#include <QTimer>
#include "settingmenu.h"
#include <QWidget>
#include <QChart>
#include <QLineSeries>
#include <QVector>
#include <QTimer>
#include "chartview.h"
#include "callout.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QValueAxis>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <macro_define.h>

#include "ModbusService/cmdprocess.h"
#include <QVector>
#include <QButtonGroup>
QT_CHARTS_USE_NAMESPACE

namespace Ui {
class DataMenu;
}


class DataMenu : public QWidget
{
    Q_OBJECT

public:
    explicit DataMenu(QWidget *parent = 0);
    ~DataMenu();

    void DataClean();
    void lineClear();

protected:
    void showEvent(QShowEvent *event);


private slots:
    void refreshData();
    void updatastau();
     void updatastau2();
    //void on_setBtn_clicked();
    void timerSlot();
    void on_dial_valueChanged(int value);
    void tipSlot(QPointF position, bool isHovering);

    void on_setBtn_clicked();
    void on_setZeroBtn_clicked();
    void on_setMotorEnableBtn_clicked();
    void on_cmdStopStartBtn_clicked();
    void on_lineClearBtn_clicked();
    void on_pushButtonUsartStart_clicked();
    void on_cmdContinueBtn_clicked();
    void on_pushButtonDataClean_clicked();
    void serialPlcReciveMannage();

    void on_pushButtonMagneticFieldToZero_clicked();

    void on_pushButtonMotorCheck_clicked();

    void on_pushButtonGoTest_clicked();


    void GroupMcuUartRadioClick();
    void GroupPlcUartRadioClick();
    void on_setBtnOutPutData_clicked();

    void on_pushButtonChangeTabWidget_clicked();

    void on_pushButtonWarningZero_clicked();

    void on_lineEditLineNumChange_textChanged(const QString &arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_verticalSlider_valueChanged(int value);

    void on_pushButtonGOGOGO_clicked();

    void on_pushButtonResetvaluesc_clicked();

private:
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

    void initDataMenuWnd();
    void initChart();
    void initTableWidget();
    void initTableWidget_2();
    void initOtherValue();
    void initSerialPortBox();
    void initRadioButtonUart();
    bool initPlcSerialService();
    bool initMcuSerialService();

    void refreshTime();
    void updateDataCharts();


private:
    QTimer timerDate;
    Ui::DataMenu *ui;
    int timeCount;  //系统计数器
    int runTime;    //系统运行时间



    ChartView *chartView;
    QChart *chart;
    Callout *tip;
    QLineSeries *series1;
    QLineSeries *series2;
    QTimer *timerChart;
    QTimer *timer_updatastatue;
    quint16 count;
    bool isStopping;

#ifdef PLC_TEST_SEND
    QSerialPort *serial;
    CmdProcess *qcmdProcess;
#endif
    QMap<DataDef::PlcCmdType, DataDef::DataStruct>tempPlcMap;
    QByteArray byteArrayReciveBuff;

    bool flagUartOpen;
    bool flagUartOpenMcu;
    bool flagUartOpenPlc;
    QButtonGroup *btnGroupUartMcu;
    QButtonGroup *btnGroupUartPlc;

    void TEST0();
    void TEST1();
    void TEST2();
    void TEST3();
    void TEST4();
    void TEST5();
    QVector<QPointF> test_data;

};

#endif // DATAMENU_H
