#ifndef CPCLIENT_H
#define CPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRegExpValidator>
#include <QStyle>
#include <QStandardItem>
#include <QJsonArray>
#include <QFileDialog>

#include "calc.h"

namespace Ui {
class CPclient;
}

class CPclient : public QWidget
{
    Q_OBJECT

public:
    explicit CPclient(QWidget *parent = nullptr);
    ~CPclient();

    QTcpSocket* socket;

//    QJsonDocument doc;
//    QJsonParseError docError;
//    bool status = NULL;
//    int requireSize;

private:
    QString qstr_ipaddres;
    QString qstr_login;
    QString qstr_password;
    QByteArray Data;

    QJsonDocument doc;
    QJsonParseError docError;
    bool bl_logStatus = NULL;
    bool bl_conStatus = NULL;
    int requireSize;


public slots:
    void sockReady();
    void sockDisc();

private slots:
    void on_Connection_Button_clicked();

    void on_LoginButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_CalcButton_clicked();

private:
    Ui::CPclient *ui;
};

#endif // CPCLIENT_H
