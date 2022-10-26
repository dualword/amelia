#include <QApplication>
#include <QDir>
#include "AMELIA.h"

#include "config.h"

int main(int argc, char* argv[])
{
  QApplication app(argc,argv);
  app.setApplicationName("AMELIA");
  
  AMELIA amelia;
  
  amelia.addSearchPath("plugins");
  amelia.addSearchPath(".");
  amelia.addSearchPath(PLUGIN_PREFIX);

  // VC++ likes to place the binaries into Debug/Release folders
  // Stolen from Qt's Echo Plugin example
  QDir appDir(app.applicationDirPath());
#if defined(Q_OS_WIN)
  QString compileType=appDir.dirName().toLower();
  if (compileType == "debug" || compileType == "release")
    amelia.addSearchPath("plugins/"+compileType);
#elif defined(Q_OS_MAC)
  if (appDir.dirName() == "Debug")
    { //XCode modifications
      amelia.addSearchPath("plugins/Debug");
      QDir cwd=QDir::current();
      cwd.cdUp();
      QDir::setCurrent(cwd.path());
    }    
#endif
  
  amelia.loadAllPlugins();
  
  return app.exec();
}
