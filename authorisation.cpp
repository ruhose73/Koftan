#include "authorisation.h"
#include "ui_authorisation.h"
#include "cpclient.h"



Authorisation::Authorisation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Authorisation)
{
    ui->setupUi(this);

//    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
//    QRegExp ipRegex ("^" + ipRange
//                     + "\\." + ipRange
//                     + "\\." + ipRange
//                     + "\\." + ipRange + "$");
//    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
//    /* ������������� ��������� �� QLineEdit */
//    ui->IPline->setValidator(ipValidator);

//    socket = new QTcpSocket(this);
//    connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
//    connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
//
}

Authorisation::~Authorisation()
{
    delete ui;
}


void Authorisation::on_loginbutton_clicked()
{
//    QString login = ui->loginline->text();//����� ��������� �����
//    QString password = ui->passwordline->text();//����� ��������� ������
//    QString ipaddres = ui->IPline->text();//����� ��������� ����� ��� �����������
//    this->close();
//    Clientwindow = new CPclient;

//    Clientwindow->IPADDRES(ipaddres, login, password);
//    Clientwindow->SOCKET(socket);

//    Clientwindow->show();
}
