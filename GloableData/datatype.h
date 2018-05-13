#ifndef DATATYPE_H
#define DATATYPE_H
#include <QString>

enum Parity : char
{
    NONE = 'N',
    ODD  = 'O',
    EVEN = 'E'
};
enum RadioButtonMcuId : int
{
    RADIOBUTTON_CLOSE_MCU = 0,
    RADIOBUTTON_OPEN_MCU  = 1,

};
enum RadioButtonPlcd : int
{

    RADIOBUTTON_CLOSE_PLC = 0,
    RADIOBUTTON_OPEN_PLC  = 1,
};




struct UartPara
{
public:
    static const unsigned short SIZE = 16;

    char port[SIZE];
    int baudRate;
    uint8_t dataBits;
    uint8_t stopBits;
    Parity parity;

    UartPara() {
        memset(port, 0, SIZE);
    }
};

#endif // DATATYPE_H
