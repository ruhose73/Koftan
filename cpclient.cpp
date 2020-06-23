#include "cpclient.h"
#include "ui_cpclient.h"


CPclient::CPclient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPclient)
{
    ui->setupUi(this);

    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->IPline->setValidator(ipValidator);

    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));

    QFile PathfileJson("LocalConfig.json");
    if (PathfileJson.open(QIODevice::ReadOnly))
    {
        QByteArray qba_PathDB = PathfileJson.readAll();
        QJsonDocument qjd_PathDB(QJsonDocument::fromJson(qba_PathDB));
        qstr_PathDB = qjd_PathDB.object().value("path").toString();
    }
    else
    {
        qDebug()<<"Файл не открыт";
    }

    LocalDataBase = QSqlDatabase::addDatabase("QSQLITE");
    LocalDataBase.setDatabaseName(qstr_PathDB);
    LocalSqlQuery = QSqlQuery(LocalDataBase);
    if(LocalDataBase.open())
    {
        for(const QString &tablename : LocalDataBase.tables())
            qDebug()<<tablename;
    }
    else
    {
        qDebug()<<"BD didn't open";
    }
}

CPclient::~CPclient()
{
    delete ui;
}

//      ("{\"type\":\"connect\",\"value\":\"true\"}")                                       статус подключения
//      ("{\"type\":\"status\",\"value\":\"true\"}")                                        статус авторизации
//      ("{\"type\":\"auth\",\"login\":\""+login+"\",\"password\":\""+password+"\"}")       авторизация
//      ("{\"type\":\"data\",\"cost\":\""+cost+"\",\"profit\":\""+profit+"\"}");            обычные данные

void CPclient::on_Connection_Button_clicked()
{
    qstr_ipaddres = ui->IPline->text();
    socket->connectToHost(qstr_ipaddres,49100);
}

void CPclient::sockDisc()
{
    socket->deleteLater();
}

void CPclient::sockReady()
{
    socket->waitForConnected(500);
    socket->waitForReadyRead(500);
    qba_Data = socket->readAll();
    qjd_doc = QJsonDocument::fromJson(qba_Data,&qjpe_docError);

    qDebug()<<qba_Data;

    if(qjpe_docError.errorString().toInt()==QJsonParseError::NoError)
    {
        if(qjd_doc.object().value("type").toString()=="connect")
        {
            if(qjd_doc.object().value("value").toString()=="true")
            {
                qDebug()<<("Соединение успешно");
                ui->statusline->clear();
                ui->statusline->setText("Соединение успешно");
                bl_conStatus = true;
            }
            else
            {
                qWarning("Соединение не успешно");
                ui->statusline->clear();
                ui->statusline->setText("Соединение не успешно");
                bl_conStatus = false;
            }
        }
        else if(qjd_doc.object().value("type").toString()=="status")
        {

            if(qjd_doc.object().value("value").toString()=="true")
            {
                qDebug()<<("Авторизация успешна");
                ui->statusline->clear();
                ui->statusline->setText("Авторизация успешна");
                ui->passwordline->clear();
                ui->loginline->clear();
                QMessageBox::information(this,"Информация","Авторизация успешна");
                bl_logStatus = true;
            }
            else
            {
                qWarning("Авторизация не успешна");
                ui->statusline->clear();
                ui->statusline->setText("Авторизация не успешна");
                ui->passwordline->clear();
                ui->loginline->clear();
                QMessageBox::information(this,"Информация","Неверный пароль или логин");
                bl_logStatus = false;
            }
        }
        else if(qjd_doc.object().value("type").toString() == "resultSelect")
        {
            qDebug() << "Получено: "<< qba_Data.size() << " Всего: " << int_requireSize;
            if(qba_Data.size() == int_requireSize)
            {
                QStandardItemModel* model = new QStandardItemModel(nullptr);
                model->setHorizontalHeaderLabels(QStringList() << "NAME"
                                                 << "DCtotal" << "WCtotal" << "PEtotal" << "AEWtotal" << "PERtotal"
                                                 << "DTBtotal" << "MATtotal" << "TCunitCost"<< "TCcostDistrib"<< "TCunitProfit");

                QJsonArray docAr = qjd_doc.object().value("NAME").toArray();
                QJsonArray docAr2 = qjd_doc.object().value("DCtotal").toArray();
                QJsonArray docAr3 = qjd_doc.object().value("WCtotal").toArray();
                QJsonArray docAr4 = qjd_doc.object().value("PEtotal").toArray();
                QJsonArray docAr5 = qjd_doc.object().value("AEWtotal").toArray();
                QJsonArray docAr6 = qjd_doc.object().value("PERtotal").toArray();
                QJsonArray docAr7 = qjd_doc.object().value("DTBtotal").toArray();
                QJsonArray docAr8 = qjd_doc.object().value("MATtotal").toArray();
                QJsonArray docAr9 = qjd_doc.object().value("TCunitCost").toArray();
                QJsonArray docAr10 = qjd_doc.object().value("TCcostDistrib").toArray();
                QJsonArray docAr11 = qjd_doc.object().value("TCunitProfit").toArray();

                for(int i =0; i<docAr.count(); i++)
                {
                    QList<QStandardItem*> q_list;
                    QStandardItem* col = new QStandardItem(docAr[i].toObject().value("name").toString());
                    QStandardItem* col2 = new QStandardItem(docAr2[i].toObject().value("dctotal").toString());
                    QStandardItem* col3 = new QStandardItem(docAr3[i].toObject().value("wctotal").toString());
                    QStandardItem* col4 = new QStandardItem(docAr4[i].toObject().value("petotal").toString());
                    QStandardItem* col5 = new QStandardItem(docAr5[i].toObject().value("aewtotal").toString());
                    QStandardItem* col6 = new QStandardItem(docAr6[i].toObject().value("pertotal").toString());
                    QStandardItem* col7 = new QStandardItem(docAr7[i].toObject().value("dtbtotal").toString());
                    QStandardItem* col8 = new QStandardItem(docAr8[i].toObject().value("mattotal").toString());
                    QStandardItem* col9 = new QStandardItem(docAr9[i].toObject().value("tcunitcost").toString());
                    QStandardItem* col10 = new QStandardItem(docAr10[i].toObject().value("tccostdistrib").toString());
                    QStandardItem* col11 = new QStandardItem(docAr11[i].toObject().value("tcunitprofit").toString());

                    q_list << col;
                    q_list << col2;
                    q_list << col3;
                    q_list << col4;
                    q_list << col5;
                    q_list << col6;
                    q_list << col7;
                    q_list << col8;
                    q_list << col9;
                    q_list << col10;
                    q_list << col11;

                    model->appendRow(q_list);
                }

                ui->tableView->setModel(model);
            }
        }

        else if(qjd_doc.object().value("type").toString() == "resultSelect1")
        {
            qDebug() << "Получено: "<< qba_Data.size() << " Всего: " << int_requireSize;
            if(qba_Data.size() == int_requireSize)
            {
                QStandardItemModel* model = new QStandardItemModel(nullptr);
                model->setHorizontalHeaderLabels(QStringList() << "NAME"
                                                 << "DCpackaging" << "DCdelivery" << "DCloading" << "DCfeels" << "DCstorageRoom"
                                                 << "DCadvertising" << "DCentertainment" << "DCtotal"
                                                 << "NPCplain"<< "NPCdevProd"
                                                 << "NPCdamProp"<< "NPCforfeits"
                                                 << "NPClossMat"<< "NPCtotal"
                                                 << "WCdeprecation"<< "WCheating"
                                                 << "WCdeductionWage"<< "WCdeductionSalary"
                                                 << "WCtotal"<< "PEsfProducts"<< "PEprimProd"
                                                 << "PEauxiliaryProd"<< "PEoverheadCosts"
                                                 << "PEmarriage"<< "PEprodService"
                                                 << "PEtotal"<< "AEWadditIncome"
                                                 << "AEWaddEarning"<< "AEWtotal"
                                                 << "PERheating"<< "PERwater"
                                                 << "PERelectricity"<< "PERtotal"
                                                 << "DTBinsurance"<< "DTBdepreciation"
                                                 << "DTBtotal"<< "MATmainMat"
                                                 << "MATcostSf"<< "MATrelatedMat"
                                                 << "MATtotal"<< "TCunitCost"
                                                 << "TCcostDistrib"<< "TCtotal");

                QJsonArray docAr = qjd_doc.object().value("NAME").toArray();
                QJsonArray docAr2 = qjd_doc.object().value("DCpackaging").toArray();
                QJsonArray docAr3 = qjd_doc.object().value("DCdelivery").toArray();
                QJsonArray docAr4 = qjd_doc.object().value("DCloading").toArray();
                QJsonArray docAr5 = qjd_doc.object().value("DCfeels").toArray();
                QJsonArray docAr6 = qjd_doc.object().value("DCstorageRoom").toArray();
                QJsonArray docAr7 = qjd_doc.object().value("DCadvertising").toArray();
                QJsonArray docAr8 = qjd_doc.object().value("DCentertainment").toArray();
                QJsonArray docAr9 = qjd_doc.object().value("DCtotal").toArray();

                QJsonArray docAr10 = qjd_doc.object().value("NPCplain").toArray();
                QJsonArray docAr11 = qjd_doc.object().value("NPCdevProd").toArray();
                QJsonArray docAr12 = qjd_doc.object().value("NPCdamProp").toArray();
                QJsonArray docAr13 = qjd_doc.object().value("NPCforfeits").toArray();
                QJsonArray docAr14 = qjd_doc.object().value("NPClossMat").toArray();
                QJsonArray docAr15 = qjd_doc.object().value("NPCtotal").toArray();

                QJsonArray docAr16 = qjd_doc.object().value("WCdeprecation").toArray();
                QJsonArray docAr17 = qjd_doc.object().value("WCheating").toArray();
                QJsonArray docAr18 = qjd_doc.object().value("WCdeductionWage").toArray();
                QJsonArray docAr19 = qjd_doc.object().value("WCdeductionSalary").toArray();
                QJsonArray docAr20 = qjd_doc.object().value("WCtotal").toArray();

                QJsonArray docAr21 = qjd_doc.object().value("PEsfProducts").toArray();
                QJsonArray docAr22 = qjd_doc.object().value("PEprimProd").toArray();
                QJsonArray docAr23 = qjd_doc.object().value("PEauxiliaryProd").toArray();
                QJsonArray docAr24 = qjd_doc.object().value("PEoverheadCosts").toArray();
                QJsonArray docAr25 = qjd_doc.object().value("PEmarriage").toArray();
                QJsonArray docAr26 = qjd_doc.object().value("PEprodService").toArray();
                QJsonArray docAr27 = qjd_doc.object().value("PEtotal").toArray();

                QJsonArray docAr28 = qjd_doc.object().value("AEWadditIncome").toArray();
                QJsonArray docAr29 = qjd_doc.object().value("AEWaddEarning").toArray();
                QJsonArray docAr30 = qjd_doc.object().value("AEWtotal").toArray();

                QJsonArray docAr31 = qjd_doc.object().value("PERheating").toArray();
                QJsonArray docAr32 = qjd_doc.object().value("PERwater").toArray();
                QJsonArray docAr33 = qjd_doc.object().value("PERelectricity").toArray();
                QJsonArray docAr34 = qjd_doc.object().value("PERtotal").toArray();

                QJsonArray docAr35 = qjd_doc.object().value("DTBinsurance").toArray();
                QJsonArray docAr36 = qjd_doc.object().value("DTBdepreciation").toArray();
                QJsonArray docAr37 = qjd_doc.object().value("DTBtotal").toArray();

                QJsonArray docAr38 = qjd_doc.object().value("MATmainMat").toArray();
                QJsonArray docAr39 = qjd_doc.object().value("MATcostSf").toArray();
                QJsonArray docAr40 = qjd_doc.object().value("MATrelatedMat").toArray();
                QJsonArray docAr41 = qjd_doc.object().value("MATtotal").toArray();

                QJsonArray docAr42 = qjd_doc.object().value("TCunitCost").toArray();
                QJsonArray docAr43 = qjd_doc.object().value("TCcostDistrib").toArray();
                QJsonArray docAr44 = qjd_doc.object().value("TCtotal").toArray();

                for(int i =0; i<docAr.count(); i++)
                {
                    QList<QStandardItem*> q_list;
                    QStandardItem* col = new QStandardItem(docAr[i].toObject().value("name").toString());
                    QStandardItem* col2 = new QStandardItem(docAr2[i].toObject().value("dcpackaging").toString());
                    QStandardItem* col3 = new QStandardItem(docAr3[i].toObject().value("dcdelivery").toString());
                    QStandardItem* col4 = new QStandardItem(docAr4[i].toObject().value("dcloading").toString());
                    QStandardItem* col5 = new QStandardItem(docAr5[i].toObject().value("dcfeels").toString());
                    QStandardItem* col6 = new QStandardItem(docAr6[i].toObject().value("dcstorageroom").toString());
                    QStandardItem* col7 = new QStandardItem(docAr7[i].toObject().value("dcadvertising").toString());
                    QStandardItem* col8 = new QStandardItem(docAr8[i].toObject().value("dcentertainment").toString());
                    QStandardItem* col9 = new QStandardItem(docAr9[i].toObject().value("dctotal").toString());

                    QStandardItem* col10 = new QStandardItem(docAr10[i].toObject().value("npcplain").toString());
                    QStandardItem* col11 = new QStandardItem(docAr11[i].toObject().value("npcdevprod").toString());
                    QStandardItem* col12 = new QStandardItem(docAr12[i].toObject().value("npcdamprop").toString());
                    QStandardItem* col13 = new QStandardItem(docAr13[i].toObject().value("npcforfeits").toString());
                    QStandardItem* col14 = new QStandardItem(docAr14[i].toObject().value("npclossmat").toString());
                    QStandardItem* col15 = new QStandardItem(docAr15[i].toObject().value("npctotal").toString());

                    QStandardItem* col16 = new QStandardItem(docAr16[i].toObject().value("wcdeprecation").toString());
                    QStandardItem* col17 = new QStandardItem(docAr17[i].toObject().value("wcheating").toString());
                    QStandardItem* col18 = new QStandardItem(docAr18[i].toObject().value("wcdeductionwage").toString());
                    QStandardItem* col19 = new QStandardItem(docAr19[i].toObject().value("wcdeductionsalary").toString());
                    QStandardItem* col20 = new QStandardItem(docAr20[i].toObject().value("wctotal").toString());

                    QStandardItem* col21 = new QStandardItem(docAr21[i].toObject().value("pesfproducts").toString());
                    QStandardItem* col22 = new QStandardItem(docAr22[i].toObject().value("peprimprod").toString());
                    QStandardItem* col23 = new QStandardItem(docAr23[i].toObject().value("peauxiliaryprod").toString());
                    QStandardItem* col24 = new QStandardItem(docAr24[i].toObject().value("peoverheadcosts").toString());
                    QStandardItem* col25 = new QStandardItem(docAr25[i].toObject().value("pemarriage").toString());
                    QStandardItem* col26 = new QStandardItem(docAr26[i].toObject().value("peprodservice").toString());
                    QStandardItem* col27 = new QStandardItem(docAr27[i].toObject().value("petotal").toString());

                    QStandardItem* col28 = new QStandardItem(docAr28[i].toObject().value("aewadditincome").toString());
                    QStandardItem* col29 = new QStandardItem(docAr29[i].toObject().value("aewaddearning").toString());
                    QStandardItem* col30 = new QStandardItem(docAr30[i].toObject().value("aewtotal").toString());

                    QStandardItem* col31 = new QStandardItem(docAr31[i].toObject().value("perheating").toString());
                    QStandardItem* col32 = new QStandardItem(docAr32[i].toObject().value("perwater").toString());
                    QStandardItem* col33 = new QStandardItem(docAr33[i].toObject().value("perelectricity").toString());
                    QStandardItem* col34 = new QStandardItem(docAr34[i].toObject().value("pertotal").toString());

                    QStandardItem* col35 = new QStandardItem(docAr35[i].toObject().value("dtbinsurance").toString());
                    QStandardItem* col36 = new QStandardItem(docAr36[i].toObject().value("dtbdepreciation").toString());
                    QStandardItem* col37 = new QStandardItem(docAr37[i].toObject().value("dtbtotal").toString());

                    QStandardItem* col38 = new QStandardItem(docAr38[i].toObject().value("matmainmat").toString());
                    QStandardItem* col39 = new QStandardItem(docAr39[i].toObject().value("matcostsf").toString());
                    QStandardItem* col40 = new QStandardItem(docAr40[i].toObject().value("matrelatedmat").toString());
                    QStandardItem* col41 = new QStandardItem(docAr41[i].toObject().value("mattotal").toString());

                    QStandardItem* col42 = new QStandardItem(docAr42[i].toObject().value("tcunitcost").toString());
                    QStandardItem* col43 = new QStandardItem(docAr43[i].toObject().value("tccostdistrib").toString());
                    QStandardItem* col44 = new QStandardItem(docAr44[i].toObject().value("tctotal").toString());

                    q_list << col;
                    q_list << col2;
                    q_list << col3;
                    q_list << col4;
                    q_list << col5;
                    q_list << col6;
                    q_list << col7;
                    q_list << col8;
                    q_list << col9;
                    q_list << col10;
                    q_list << col11;
                    q_list << col12;
                    q_list << col13;
                    q_list << col14;
                    q_list << col15;
                    q_list << col16;
                    q_list << col17;
                    q_list << col18;
                    q_list << col19;
                    q_list << col20;
                    q_list << col21;
                    q_list << col22;
                    q_list << col23;
                    q_list << col24;
                    q_list << col25;
                    q_list << col26;
                    q_list << col27;
                    q_list << col28;
                    q_list << col29;
                    q_list << col30;
                    q_list << col31;
                    q_list << col32;
                    q_list << col33;
                    q_list << col34;
                    q_list << col35;
                    q_list << col36;
                    q_list << col37;
                    q_list << col38;
                    q_list << col39;
                    q_list << col40;
                    q_list << col41;
                    q_list << col42;
                    q_list << col43;
                    q_list << col44;

                    model->appendRow(q_list);
                }

                ui->tableView->setModel(model);
            }
        }

        else if((qjd_doc.object().value("type").toString() == "size") && (qjd_doc.object().value("resp").toString() == "workers"))
        {
            int_requireSize = qjd_doc.object().value("length").toInt();

            ui->statusline->setText("Размер: " + QString::number(int_requireSize) + " байт");

            socket->write("{\"type\":\"select\",\"params\":\"workers\"}");
            socket->waitForBytesWritten(500);
            qDebug()<<int_requireSize;
        }
        else if((qjd_doc.object().value("type").toString() == "size") && (qjd_doc.object().value("resp").toString() == "workers1"))
        {
            int_requireSize = qjd_doc.object().value("length").toInt();

            ui->statusline->setText("Размер: " + QString::number(int_requireSize) + " байт");

            socket->write("{\"type\":\"select\",\"params\":\"workers1\"}");
            socket->waitForBytesWritten(500);
            qDebug()<<int_requireSize;
        }
    }
}

void CPclient::on_LoginButton_clicked()
{
    qstr_login = ui->loginline->text();//берем введенный логин
    qstr_password = ui->passwordline->text();//берем введенный пароль

    if(bl_conStatus == true)
    {
        QByteArray qba_passlog;
        qba_passlog.append("{\"type\":\"auth\",\"login\":\""+qstr_login+"\",\"password\":\""+qstr_password+"\"}");
        socket->write(qba_passlog);
        socket->waitForBytesWritten(1000);
    }
    else
    {
        QMessageBox::information(this,"Информация","Локальная авторизация");

        if(LocalSqlQuery.exec("SELECT login, password FROM authlist WHERE login =\'" + qstr_login + "\' AND password =\'" + qstr_password + "\'"))
        {
            if(LocalSqlQuery.next())
            {
                QMessageBox::information(this,"Информация","Авторизация успешна");
                ui->statusline->setText("Авторизация успешна");
                bl_logStatus = true;
            }
            else
            {
                qDebug() << "Good auth operation.\nAuth failed.";
                QMessageBox::information(this,"Информация","Авторизация не успешна");
                ui->statusline->setText("Авторизация не успешна");
                bl_logStatus = false;
            }
        }
    }
}

void CPclient::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        QPalette darkPalette;

        // Настраиваем палитру для элементов интерфейса
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        // Устанавливаем темную палитру
        qApp->setPalette(darkPalette);
    }
    else
    {
        qApp->setPalette(style()->standardPalette());

    }

}

void CPclient::on_pushButton_clicked()
{
    QString tablename = ui->DB_table_Box->currentText();
    if(bl_logStatus == true)
    {
        if(tablename == "Предприятие")
        {
            if((socket->isOpen()) && (bl_conStatus == true))
            {
                socket->write("{\"type\":\"recSize\",\"resp\":\"workers\"}");
                socket->waitForBytesWritten(500);
            }
            else
            {
                QMessageBox::information(this,"Информация","Открывается локальная база данных");

                QSqlTableModel *modal = new QSqlTableModel;
                modal->setTable("concern");
                modal->select();
                ui->tableView->setModel(modal);
            }
        }
        else
            if((socket->isOpen()) && (bl_conStatus == true))
            {
                socket->write("{\"type\":\"recSize\",\"resp\":\"workers1\"}");
                socket->waitForBytesWritten(500);
            }
            else
            {
                QMessageBox::information(this,"Информация","Открывается локальная база данных");
                QSqlTableModel *modal = new QSqlTableModel;
                modal->setTable("concernDetail");
                modal->select();
                ui->tableView->setModel(modal);
            }
    }
    else
    {
        QMessageBox::information(this,"Информация","Вы не авторизованы");
    }

}

void CPclient::on_CalcButton_clicked()
{
    Calc *calc = new Calc;
    calc->show();
}

void CPclient::on_chooseDB_button_clicked()
{
    qstr_PathDB = QFileDialog::getOpenFileName(this, "Укажите файл базы данных", "", "Database (*db)");

    QFile PathfileJson("LocalConfig.json");
    PathfileJson.open(QIODevice::WriteOnly);
    QVariantMap testMap;
    testMap.insert("path",qstr_PathDB);
    PathfileJson.write(QJsonDocument(QJsonObject::fromVariantMap(testMap)).toJson());
    PathfileJson.close();
}

void CPclient::on_WriteCopyDB_box_stateChanged(int arg1)
{

    if(arg1 == 2)
    {
        bl_localBase = true;
    }
    else
    {
        bl_localBase = false;
    }

}
