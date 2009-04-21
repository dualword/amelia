#include <QApplication>
#include <QDir>
#include "AMELIA.h"

int main(int argc, char* argv[])
{
  QApplication app(argc,argv);
  app.setApplicationName("AMELIA");

  AMELIA amelia;

  amelia.addSearchPath("plugins");

  // VC++ likes to place the binaries into Debug/Release folders
  // Stolen from Qt's Echo Plugin example
  QDir appDir(app.applicationDirPath());
 #if defined(Q_OS_WIN)
	QString compileType=appDir.dirName().toLower();
     if (compileType == "debug" || compileType == "release")
         amelia.addSearchPath("plugins/"+compileType);
 #elif defined(Q_OS_MAC)
     if (pluginsDir.dirName() == "MacOS")
		amelia.addSearchPath("plugins/MacOS");       
 #endif
  
  amelia.loadAllPlugins();

  return app.exec();
}
