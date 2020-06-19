#include "cpclient.h"
#include "ui_cpclient.h"


CPclient::CPclient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPclient)
{
    ui->setupUi(this);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    /* Устанавливаем Валидатор на QLineEdit */
    ui->IPline->setValidator(ipValidator);

    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
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

    QString ipaddres = ui->IPline->text();
    socket->connectToHost(ipaddres,49100);
}

void CPclient::sockDisc()
{
    socket->deleteLater();
}

void CPclient::sockReady()
{
        socket->waitForConnected(500);
        socket->waitForReadyRead(500);
        Data = socket->readAll();
        doc = QJsonDocument::fromJson(Data,&docError);
        qDebug()<<Data;

        if(docError.errorString().toInt()==QJsonParseError::NoError)
        {
            if(doc.object().value("type").toString()=="connect")
            {
                if(doc.object().value("value").toString()=="true")
                {
                    qDebug()<<("Соединение успешно");
                    ui->statusline->clear();
                    ui->statusline->setText("Соединение успешно");
                }
                else
                {
                    qDebug()<<("Соединение не успешно");
                    ui->statusline->clear();
                    ui->statusline->setText("Соединение не успешно");
                }
            }
            else if(doc.object().value("type").toString()=="status")
            {

                if(doc.object().value("value").toString()=="true")
                {
                    qDebug()<<("Авторизация успешна");
                    ui->statusline->clear();
                    ui->statusline->setText("Авторизация успешна");
                    ui->passwordline->clear();
                    ui->loginline->clear();
                    QMessageBox::information(this,"Информация","Авторизация успешна");
                    status = true;
                }
                else
                {
                    qDebug()<<("Авторизация не успешна");
                    ui->statusline->clear();
                    ui->statusline->setText("Авторизация не успешна");
                    ui->passwordline->clear();
                    ui->loginline->clear();
                    QMessageBox::information(this,"Информация","Неверный пароль или логин");
                    status = false;
                }
            }
            else if(doc.object().value("type").toString() == "resultSelect")
            {
                            qDebug() << "Получено: "<< Data.size() << " Всего: " << requireSize;
                            if(Data.size() == requireSize)
                            {
                                QStandardItemModel* model = new QStandardItemModel(nullptr);


                                model->setHorizontalHeaderLabels(QStringList() << "ID"
                                << "Название" << "МО" << "МВ" << "ПФ" << "ТР" << "А"
                                << "Э" << "ВОР" << "ДЗ"<< "ОСБ"<< "РЗ"<< "ЦЗ"<< "НИ"<< "РС");

                                QJsonArray docAr = doc.object().value("ID").toArray();
                                qDebug() <<docAr;
                                QJsonArray docAr2 = doc.object().value("Name").toArray();
                                qDebug() <<docAr2;
                                QJsonArray docAr3 = doc.object().value("ACBM").toArray();
                                qDebug() <<docAr3;
                                QJsonArray docAr4 = doc.object().value("RM").toArray();
                                qDebug() <<docAr4;
                                QJsonArray docAr5 = doc.object().value("CPSFP").toArray();
                                qDebug() <<docAr5;
                                QJsonArray docAr6 = doc.object().value("TC").toArray();
                                qDebug() <<docAr6;
                                QJsonArray docAr7 = doc.object().value("D").toArray();
                                qDebug() <<docAr7;
                                QJsonArray docAr8 = doc.object().value("PER").toArray();
                                qDebug() << docAr8;
                                QJsonArray docAr9 = doc.object().value("RLMW").toArray();
                                qDebug() << docAr9;

                                QJsonArray docAr10 = doc.object().value("AEW").toArray();
                                qDebug() << docAr10;
                                QJsonArray docAr11 = doc.object().value("DIB").toArray();
                                qDebug() << docAr11;
                                QJsonArray docAr12 = doc.object().value("PE").toArray();
                                qDebug() << docAr12;
                                QJsonArray docAr13 = doc.object().value("WC").toArray();
                                qDebug() << docAr13;
                                QJsonArray docAr14 = doc.object().value("NPC").toArray();
                                qDebug() << docAr14;
                                QJsonArray docAr15 = doc.object().value("DC").toArray();
                                qDebug() << docAr15;

                                for(int i =0; i<docAr.count(); i++)
                                {
                                    QList<QStandardItem*> q_list;
                                    QStandardItem* col = new QStandardItem(docAr[i].toObject().value("id").toString());
                                    QStandardItem* col2 = new QStandardItem(docAr2[i].toObject().value("name").toString());
                                    QStandardItem* col3 = new QStandardItem(docAr3[i].toObject().value("acbm").toString());
                                    QStandardItem* col4 = new QStandardItem(docAr4[i].toObject().value("rm").toString());
                                    QStandardItem* col5 = new QStandardItem(docAr5[i].toObject().value("cpsfp").toString());
                                    QStandardItem* col6 = new QStandardItem(docAr6[i].toObject().value("tc").toString());
                                    QStandardItem* col7 = new QStandardItem(docAr7[i].toObject().value("d").toString());
                                    QStandardItem* col8 = new QStandardItem(docAr8[i].toObject().value("per").toString());
                                    QStandardItem* col9 = new QStandardItem(docAr9[i].toObject().value("rlmw").toString());
                                    QStandardItem* col10 = new QStandardItem(docAr10[i].toObject().value("aew").toString());
                                    QStandardItem* col11 = new QStandardItem(docAr11[i].toObject().value("dib").toString());
                                    QStandardItem* col12 = new QStandardItem(docAr12[i].toObject().value("pe").toString());
                                    QStandardItem* col13 = new QStandardItem(docAr13[i].toObject().value("wc").toString());
                                    QStandardItem* col14 = new QStandardItem(docAr14[i].toObject().value("npc").toString());
                                    QStandardItem* col15 = new QStandardItem(docAr15[i].toObject().value("dc").toString());

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

                                    model->appendRow(q_list);
                                }
                                ui->tableView->setModel(model);
                            }
                        }

            else if((doc.object().value("type").toString() == "size") && (doc.object().value("resp").toString() == "workers"))
            {
                requireSize = doc.object().value("length").toInt();

                ui->statusline->setText("Размер: " + QString::number(requireSize) + " байт");

                socket->write("{\"type\":\"select\",\"params\":\"workers\"}");
                socket->waitForBytesWritten(500);
                qDebug()<<requireSize;
            }
        }
}

void CPclient::on_LoginButton_clicked()
{
    QString login = ui->loginline->text();//берем введенный логин
    QString password = ui->passwordline->text();//берем введенный пароль
    QByteArray passlog;
    passlog.append("{\"type\":\"auth\",\"login\":\""+login+"\",\"password\":\""+password+"\"}");
    socket->write(passlog);
    socket->waitForBytesWritten(1000);
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
    if(status == true)
    {
        if(socket->isOpen())
        {
            socket->write("{\"type\":\"recSize\",\"resp\":\"workers\"}");
            socket->waitForBytesWritten(500);
        }
        else
        {
            QMessageBox::information(this,"Информация","Нет соединения с сервером");
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
