#if 1
#ifndef UTILS_H
#define UTILS_H
#include <QByteArray>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <QCoreApplication>
#include <QTime>



class Utils
{
public:
    Utils();
    static char heToAsciiNum(char hex);
    static bool lrcCheck( QByteArray ReciveBuff);//only for plc motor
    static unsigned char lrcCaculate( char*,int);//only for plc motor
    static void writeLogTXT(QString content,QString file,int line,QString func);
    static sleep(unsigned int msec);
};

#endif // UTILS_H
#endif
