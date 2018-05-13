#include "databaseengine.h"

MYDataBase::DataBase()
{

}


//建立一个数据库连接
bool MYDataBase::createConnection(QString db_addr)
{
    //以后就可以用"sqlite1"与数据库进行连接了
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "sqlite1");
    db.setDatabaseName(db_addr);
    if( !db.open())
    {
        qDebug() << "createConnection失败";
        return false;
    }
     qDebug() << "  createConnection成功！！";
    return true;
}

//创建数据库表

bool MYDataBase::createTable(QString db_create_cmd)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    bool success = query.exec(      db_create_cmd    );


    if(success)
    {
        qDebug() << QObject::tr("createTable成功！\n");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("createTable失败！\n");
        return false;
    }
}

//向数据库中插入记录
//bool DataBase::insert()
//{
//    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
//    QSqlQuery query(db);


//#if 1   //这里两种写法
//    query.prepare("insert into point_table values(?, ?, ?)");

//    long records = 10;

//    for(int i=0; i<records; i++)
//    {
//        query.bindValue(0, i);
//        query.bindValue(1, "李佳豪");
//        query.bindValue(2, "陕西西安");

//#else

//        query.prepare("insert into point_table values(:id,:name,:addr)");

//        long records = 10;

//        for(int i=0; i<records; i++)
//        {
//            query.bindValue(":id", i);
//            query.bindValue(":name", "李佳豪");
//            query.bindValue(":addr", "陕西西安");
//#endif
//        bool success=query.exec();
//        if(!success)
//        {
//            QSqlError lastError = query.lastError();
//            qDebug() << lastError.driverText() << QString(QObject::tr("insert失败"));
//            return false;
//        }
//    }
//    return true;
//}


//向数据库中插入记录
bool MYDataBase::insert(int id,double x,double y,QString info)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

            query.prepare("insert into point_table1 values(?, ?, ?, ?)");

            query.bindValue(0, id);
            query.bindValue(1, x);
            query.bindValue(2, y);
             query.bindValue(3, info);

        bool success=query.exec();
        if(!success)
        {
            QSqlError lastError = query.lastError();
            qDebug() << lastError.driverText() << QString(QObject::tr("insert失败"));
            return false;
        }

        return true;
}




#if 0
//查询所有信息
bool DataBase::queryAll()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.exec("select * from point_table");
    QSqlRecord rec = query.record();
    qDebug() << QObject::tr("point_table表字段数：" ) << rec.count();
    qDebug() << QObject::tr("point_table表字段数：" ) << rec.fieldName(0);
    qDebug() << QObject::tr("point_table表字段数：" ) << rec.fieldName(1);
    qDebug() << QObject::tr("point_table表字段数：" ) << rec.fieldName(2);


    while(query.next())
    {
             for(int index = 0; index < 3; index++)
             qDebug() << query.value(index) << " ";
             qDebug() << "_____________________\n";
    }


}




//根据ID删除记录
bool DataBase::deleteById(int id)
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    query.prepare(QString("delete from point_table where id = %1").arg(id));
    if(!query.exec())
    {
        qDebug() << "删除记录失败！";
        return false;
    }
       qDebug() << "删除id 成功！";
    return true;
}

//根据ID更新记录  改
bool DataBase::updateById(int id)
{
       QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
       QSqlQuery query(db);
       query.prepare(QString("update point_table set name=:tt,addr=:xx where id=%1").arg(id));


        query.bindValue(":tt","张凡");
        query.bindValue(":xx","美国洛杉矶");


        bool success=query.exec();
        if(!success)
        {
             QSqlError lastError = query.lastError();
             qDebug() << lastError.driverText() << QString(QObject::tr("更新失败"));
        }
       return true;
}

//排序
bool DataBase::sortById()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);
    bool success=query.exec("select * from point_table order by id desc");
    if(success)
    {
        qDebug() << QObject::tr("排序成功");
        return true;
    }
    else
    {
        qDebug() << QObject::tr("排序失败！");
        return false;
    }
}

bool DataBase::getMaxById()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

    bool success=query.exec("select max(id) from point_table");

    if(success)
    {
        qDebug() << QObject::tr("getmaxcmd成功,开始检索..");
        while(query.next())
            {
               qDebug() <<"max id is :"<< query.value(0).toInt();
            }
        return true;
    }
    else
    {
        qDebug() << QObject::tr("getmaxcmd失败！");
        return false;
    }

}

bool DataBase::getMinById()
{
    QSqlDatabase db = QSqlDatabase::database("sqlite1"); //建立数据库连接
    QSqlQuery query(db);

    bool success=query.exec("select min(id) from point_table");

    if(success)
    {
        qDebug() << QObject::tr("getmincmd成功,开始检索..");
        while(query.next())
            {
               qDebug() <<"min id is :"<< query.value(0).toInt();
            }
        return true;
    }
    else
    {
        qDebug() << QObject::tr("getmincmd失败！");
        return false;
    }

}
#endif



