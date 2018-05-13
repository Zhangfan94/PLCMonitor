/*
 这里放测试代码
*/


#if 0


//     QByteArray xx;
//     xx.append('8');
//      xx.append('C');
//       xx.append('A');
//        xx.append('0');
//  qDebug()<< "QByteArray: " <<xx;
//  qDebug()<< "QString   : " <<QString(xx);



//      QString str = QString(xx);
//      bool ok;
//      int hex = str.toInt(&ok, 16);       // hex == 255, ok == true

//      qDebug()<<hex;
//      qDebug()<<ok;
//     qDebug()<< QString::number(hex,10);

    //打开文件，取得工作簿
          //  QExcel j("D:/test.xls");
        //取得工作表数量
          // qDebug()<<"SheetCount"<<j.getSheetsCount();
        //激活一张工作表
        //j.selectSheet(1);
        //j.selectSheet("JSheet2");
        //取得工作表名称
        //j.selectSheet(1);
        //j.getSheetName();
        //qDebug()<<"SheetName 1"<<j.getSheetName(1);
        //取得工作表已使用范围
        //int topLeftRow, topLeftColumn, bottomRightRow, bottomRightColumn;
        //j.getUsedRange(&topLeftRow, &topLeftColumn, &bottomRightRow, &bottomRightColumn);
        //读值
        //j.getCellValue(2, 2).toString();
        //删除工作表
        //j.selectSheet("Sheet1");
        //j.selectSheet(1);
        //j.deleteSheet();
        //j.save();
        //插入数据
//            j.selectSheet("Sheet1");
//            j.setCellString(1, 7, "addString");
//            j.setCellString("A3", "abc");
         //   j.save();
        //合并单元格
        //j.selectSheet(2);
        //j.mergeCells("G1:H2");
        //j.mergeCells(4, 7, 5 ,8);
        //j.save();
        //设置列宽
        //j.selectSheet(1);
        //j.setColumnWidth(1, 20);
        //j.save();
        //设置粗体
        //j.selectSheet(1);
        //j.setCellFontBold(2, 2, true);
        //j.setCellFontBold("A2", true);
        //j.save();
        //设置文字大小
        //j.selectSheet(1);
        //j.setCellFontSize("B3", 20);
        //j.setCellFontSize(1, 2, 20);
        //j.save();
        //设置单元格文字居中
        //j.selectSheet(2);
        //j.setCellTextCenter(1, 2);
        //j.setCellTextCenter("A2");
        //j.save();
        //设置单元格文字自动折行
        //j.selectSheet(1);
        //j.setCellTextWrap(2,2,true);
        //j.setCellTextWrap("A2", true);
        //j.save();
        //设置一行自适应行高
        //j.selectSheet(1);
        //j.setAutoFitRow(2);
        //j.save();
        //新建工作表
        //j.insertSheet("abc");
        //j.save();
        //清除单元格内容
        //j.selectSheet(4);
        //j.clearCell(1,1);
        //j.clearCell("A2");
        //j.save();
        //合并一列中相同连续的单元格
        //j.selectSheet(1);
        //j.mergeSerialSameCellsInColumn(1, 2);
        //j.save();
        //获取一张工作表已用行数
        //j.selectSheet(1);
        //qDebug()<<j.getUsedRowsCount();
        //设置行高
            //j.selectSheet(1);
            //j.setRowHeight(2, 30);
            //j.save();

      //  j.close();
       // qDebug()<<"App End";


void TEST_execlout3(void)
{

    ExcelEngine excel(QObject::tr("./../Desktop/MonitorReport_1.xls")); //创建
    excel.Open(); //打开




    excel.mergeCells("A1:A2");
    excel.SetCellData(1,1,"序号");
    excel.setCellTextCenter(1,1);
    //  excel.setCellTextColor(5,1);

    excel.mergeCells("B1:B2");
    excel.SetCellData(1,2,"磁极数");
    excel.setCellTextCenter(1,2);

    excel.mergeCells("C1:D1");
    excel.SetCellData(1,3,"磁场最大值(mT)");
    excel.setCellTextCenter(1,3);

    excel.mergeCells("E1:F1");
    excel.SetCellData("E1:F1","磁场最小值(mT)");
    excel.setCellTextCenter("E1:F1");

    excel.mergeCells("G1:H1");
    excel.SetCellData("G1:H1","磁场平均值(mT)");
    excel.setCellTextCenter("G1:H1");

    excel.mergeCells("I1:J2");
    excel.SetCellData("I1:J2","磁场偏差(mT)");
    excel.setCellTextCenter("I1:J2");

    excel.mergeCells("K1:L1");
    excel.SetCellData("K1:L1","宽度最大值(mT)");
    excel.setCellTextCenter("K1:L1");

    excel.mergeCells("M1:N1");
    excel.SetCellData("M1:N1","宽度最小值(mT)");
    excel.setCellTextCenter("M1:N1");

    excel.mergeCells("O1:P1");
    excel.SetCellData("O1:P1","宽度平均值(mT)");
    excel.setCellTextCenter("O1:P1");


    excel.mergeCells("Q1:R2");
    excel.SetCellData("Q1:R2","宽度偏差(mm)");
    excel.setCellTextCenter("Q1:R2");


    excel.SetCellData("C2","N");
    excel.setCellTextCenter("C2");

    excel.SetCellData("D2","S");
    excel.setCellTextCenter("D2");

    excel.SetCellData("E2","N");
    excel.setCellTextCenter("E2");

    excel.SetCellData("F2","S");
    excel.setCellTextCenter("F2");

    excel.SetCellData("G2","N");
    excel.setCellTextCenter("G2");

    excel.SetCellData("H2","S");
    excel.setCellTextCenter("H2");


    excel.SetCellData("K2","N");
    excel.setCellTextCenter("K2");

    excel.SetCellData("L2","S");
    excel.setCellTextCenter("L2");

    excel.SetCellData("M2","N");
    excel.setCellTextCenter("M2");

    excel.SetCellData("N2","S");
    excel.setCellTextCenter("N2");

    excel.SetCellData("O2","N");
    excel.setCellTextCenter("O2");

    excel.SetCellData("P2","S");
    excel.setCellTextCenter("P2");

    excel.Save(); //保存
    excel.Close();
    qDebug()<<"file output OK!!";
    //  QMessageBox::about(NULL, "报告", QString("报表已发送至桌面"));



}
void TEST_execlout4(void)
{

    QString time  = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ExcelEngine excel(QObject::tr("./../Desktop/MonitorReport_2.xls")); //创建
    excel.Open(); //打开

    excel.mergeCells("A1:B1");
    excel.SetCellData("A1:B1","最大磁场：");
    excel.setCellTextCenter("A1:B1");
    //  excel.setCellTextColor(5,1);


    excel.mergeCells("A2:B2");
    excel.SetCellData("A2:B2","最小磁场：");
    excel.setCellTextCenter("A2:B2");

    excel.mergeCells("A3:B3");
    excel.SetCellData("A3:B3","最大极宽：");
    excel.setCellTextCenter("A3:B3");

    excel.mergeCells("A4:B4");
    excel.SetCellData("A4:B4","最小极宽：");
    excel.setCellTextCenter("A4:B4");



    excel.mergeCells("D1:E1");
    excel.SetCellData("D1:E1","平均值：");
    excel.setCellTextCenter("D1:E1");
    //  excel.setCellTextColor(5,1);


    excel.mergeCells("D2:E2");
    excel.SetCellData("D2:E2","偏差值：");
    excel.setCellTextCenter("D2:E2");

    excel.mergeCells("D3:E3");
    excel.SetCellData("D3:E3","平均值：");
    excel.setCellTextCenter("D3:E3");

    excel.mergeCells("D4:E4");
    excel.SetCellData("D4:E4","偏差值：");
    excel.setCellTextCenter("D4:E4");


    excel.mergeCells("A5:A6");
    excel.SetCellData("A5:A6","极数：");
    excel.setCellTextCenter("A5:A6");

    excel.mergeCells("B5:C5");
    excel.SetCellData("B5:C5","磁场强度(mT)");
    excel.setCellTextCenter("B5:C5");

    excel.mergeCells("D5:E5");
    excel.SetCellData("D5:E5","磁场宽度(mm)");
    excel.setCellTextCenter("D5:E5");

    excel.SetCellData("B6","N");
    excel.setCellTextCenter("B6");

    excel.SetCellData("C6","S");
    excel.setCellTextCenter("C6");

    excel.SetCellData("D6","N");
    excel.setCellTextCenter("D6");

    excel.SetCellData("E6","S");
    excel.setCellTextCenter("E6");


    excel.Save(); //保存
    excel.Close();
    qDebug()<<"file output OK!!";
    //  QMessageBox::about(NULL, "报告", QString("报表已发送至桌面"));



}
void TEST_execlout(void)
{
    ExcelEngine excel(QObject::tr("./../Desktop/MonitorReport.xls")); //创建
    excel.Open(); //打开
    QVector<QString> strArray;
    strArray<<"测到的磁极数"<<"N极磁场最大值"<<"N极磁场最小值"<<"N极磁场平均值"<<"S极磁场最大值"\
           <<"S极磁场最小值"<<"S极磁场平均值"<<"实时磁场值AB"<<"实时磁场值零位"<<"磁场最大值零位";
    for(int row = 1;row<2;row++ )
    {
        for(int clomn = 1;clomn<=10;clomn++)
        {
            excel.SetCellData(row,clomn,strArray.at(clomn-1)); //修改指定单元数据
        }
    }
    // QVariant data = excel.GetCellData(11,11); //访问指定单元格数据
    // qDebug()<<data;
    excel.Save(); //保存
    excel.Close();
    qDebug()<<"file output OK!!";
    QMessageBox::about(NULL, "报告", QString("报表已发送至桌面"));
}


#endif
