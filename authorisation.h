#include <QMainWindow>
#include "cpclient.h"
#include <QMessageBox>
#include <QRegExpValidator>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Authorisation; }
QT_END_NAMESPACE

class Authorisation : public QMainWindow
{
    Q_OBJECT

public:
    Authorisation(QWidget *parent = nullptr);
    ~Authorisation();

private slots:
    void on_loginbutton_clicked();

private:
    Ui::Authorisation *ui;
    CPclient *Clientwindow;
};

