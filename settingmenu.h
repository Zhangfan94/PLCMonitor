#ifndef SETTINGMENU_H
#define SETTINGMENU_H

#include <QDialog>
#include <QSerialPort>
#include <QTimer>
#include "utils.h"
namespace Ui {
class SettingMenu;
}

class SettingMenu : public QDialog
{
    Q_OBJECT

public:
    explicit SettingMenu(QWidget *parent = 0);
    ~SettingMenu();

    void serialCheck();
    void initProgressBar();
    void textEditShow(QByteArray);


    /**
      *@brief 基于模版协议帧 进行包装（该函数需要根据情况进行修改！）
      *@param array : 基于模版协议帧
      *       array_length ：基于模版协议帧数组长度
      *       linedit_hex_get：linedit 独到的 且转为hex 的Qstring
      *@return
      */
    void sendArrayConfig(char* array,int array_length,QString linedit_hex_get);



    void TEST7();


private slots:
    void on_retBtn_clicked();

    void on_pushButton_Test_clicked();

    void settingMenuPlcRead();
    void requestByTimer();

    void on_pushButtonMotorParaApply_clicked();

    void on_pushButtonMcuCollectionParaApply_clicked();

    void on_pushButtonMotorTest_clicked();

    void on_pushButtonTest_clicked();
    void on_pushButtonMotorParaApply_2_clicked();

protected:
    void showEvent(QShowEvent *event);


private:
    Ui::SettingMenu *ui;
    QByteArray mainByteArray;
    QSerialPort *serial;
    QTimer timerGetPara;
    int Flag_Get_Para;

};

#endif // SETTINGMENU_H
