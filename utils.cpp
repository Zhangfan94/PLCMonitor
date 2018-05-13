#if 1

#include "utils.h"



Utils::Utils()
{


}

char Utils::heToAsciiNum(char hex)
{
    if((hex >= 0) && (hex <= 9))
     {
        return hex+0x30;
     }
     else if((hex >= 10) && (hex <= 15))
     {
         return hex+0x37;
     }
     else return (-1);
}

bool Utils::lrcCheck(QByteArray ReciveBuff)
{
    unsigned char sumItem = 0;
    for(int i = 0;i<ReciveBuff.size()-3;i++)
    {
        sumItem = sumItem+ReciveBuff.at(i);
    }

    if(heToAsciiNum(sumItem>>4) == ReciveBuff.at(ReciveBuff.size()-3) && heToAsciiNum(sumItem&(0x0f)) == ReciveBuff.at(ReciveBuff.size()-2) )
    {

        return 1;
    }
   return 0;

}

unsigned char Utils::lrcCaculate(char *pbuff,int lenth  )
{
      unsigned char sumItem = 0;
      for(int i = 0;i<lenth-3;i++)

    {
        sumItem = sumItem+pbuff[i];
    }
   return  sumItem;
}


void Utils::writeLogTXT(QString content,QString wfile, int line ,QString func)
{
   // QFile file("D:/Monitor_Log"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss")+".log");
    QFile file("D:/Monitor_Log_PointGet3600.log");
    QString time  = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");

    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << content << " ---"<<  "file:"<<wfile<<"   line:"<<line<<"   func:"<<func<<"   time:"<<time<<"\r\n";
    file.flush();
    file.close();
}

Utils::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}





#endif
