#include "authorisation.h"
#include "cpclient.h"

#include <QApplication>
#include <QStyleFactory>
#include <QMenuBar>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);


   qApp->setStyle(QStyleFactory::create("Fusion"));

   CPclient client;
   client.show();

//   Authorisation auth;
//   auth.show();


   return a.exec();
}
