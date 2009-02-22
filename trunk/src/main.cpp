#include <QApplication>
#include "AMELIA.h"

int main(int argc, char* argv[])
{
  QApplication app(argc,argv);
  app.setApplicationName("AMELIA");

  AMELIA amelia;

  amelia.addSearchPath("plugins/");
  amelia.loadAllPlugins();

  return app.exec();
}
