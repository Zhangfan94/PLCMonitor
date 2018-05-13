#ifndef UARTSETTING_H
#define UARTSETTING_H

#include "datamenu.h"

#include <QShowEvent>
#include <QWidget>

#include "settingmenu.h"


namespace Ui {
class UartSetting;
}

class UartSetting : public QWidget
{
    Q_OBJECT

public:
    explicit UartSetting(QWidget *parent = 0);
    ~UartSetting();

private:
    bool initPlcSerialService();
    bool initMcuSerialService();

protected:
    void showEvent(QShowEvent *event);

private slots:
    void on_loginBtn_clicked();

    void on_retBtn_clicked();

private:
    Ui::UartSetting *ui;
    DataMenu* dataMenu;
};

#endif // UARTSETTING_H
