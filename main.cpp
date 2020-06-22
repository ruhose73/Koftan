#include "cpclient.h"
#include "readme_file_class.h"

#include <QApplication>
#include <QStyleFactory>
#include <QMenuBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qApp->setStyle(QStyleFactory::create("Fusion"));
    readme_file_class readme;
    readme.writefile();
    CPclient client;
    client.show();
    return a.exec();
}
