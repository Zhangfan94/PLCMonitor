#ifndef DATABASEENGINE_H
#define DATABASEENGINE_H

#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QString>

class MYDataBase
{
public:
    DataBase();
public:
                         //".//test.db"
    bool createConnection(QString db_addr);  //创建一个连接
                     // "create table point_table(id int,name text,addr text)"
    bool createTable(QString db_create_cmd);       //创建数据库表
    bool insert(int id,double x,double y,QString info);            //出入数据
//    bool queryAll();          //查询所有信息
//    bool updateById(int id);  //更新
//    bool deleteById(int id);  //删除
//    bool sortById();          //排序

//     bool getMaxById();
//     bool getMinById();



};

#endif // DATABASEENGINE_H
