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
#include <QFile>
#include <QtSql>
#include <QSqlQuery>
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

private:

    QTcpSocket* socket;

    QString qstr_ipaddres;

    QString qstr_login;

    QString qstr_password;

    QByteArray qba_Data;

    QJsonDocument qjd_doc;
    QJsonDocument qjd_path;

    QJsonParseError qjpe_docError;

    bool bl_logStatus = NULL;

    bool bl_conStatus = NULL;

    bool bl_localBase = true;

    int int_requireSize;

    QString qstr_PathDB;

    QSqlDatabase LocalDataBase;
    QSqlQuery LocalSqlQuery;


public slots:

    void sockReady();

    void sockDisc();

private slots:

    void on_Connection_Button_clicked();

    void on_LoginButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_CalcButton_clicked();

    void on_chooseDB_button_clicked();

    void on_WriteCopyDB_box_stateChanged(int arg1);

private:

    Ui::CPclient *ui;
};

#endif // CPCLIENT_H
