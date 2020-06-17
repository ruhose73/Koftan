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
//      ("{\"type\":\"auth\",\"login\":\""+login+"\",\"password\":\""+password+"\"}")       авторизация
//      ("{\"type\":\"status\",\"value\":\"true\"}")                                        статус авторизации
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
                }
                else
                {
                    qDebug()<<("Авторизация не успешна");
                    ui->statusline->clear();
                    ui->statusline->setText("Авторизация не успешна");
                    ui->passwordline->clear();
                    ui->loginline->clear();
                    QMessageBox::information(this,"Информация","Неверный пароль или логин");
                }
            }
            else
            {
                qDebug()<<"no auth";
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
        // Создаём палитру для тёмной темы оформления
        QPalette darkPalette;

        // Настраиваем палитру для цветовых ролей элементов интерфейса
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

        // Устанавливаем данную палитру
        qApp->setPalette(darkPalette);
    }
    else
    {
        qApp->setPalette(style()->standardPalette());

    }

}
