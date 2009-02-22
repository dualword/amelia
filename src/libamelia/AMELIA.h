#ifndef AMELIA_H_
#define AMELIA_H_

#include <QString>
#include <QObject>
#include <QMap>

#include "APlugin.h"

class LIBAMELIA_EXPORT AMELIA : public QObject
{
  Q_OBJECT

 public:
  static AMELIA *global;

  AMELIA(QObject *parent=0);

  void loadAllPlugins();
  bool loadPlugin(QString name);
  bool isLoaded(QString name);
  APlugin* plugin(QString plugin);

  void addSearchPath(QString);

 signals:
  void allPluginsLoaded();
  void pluginLoaded(QString);
  void pluginUnLoaded(QString);

 private:
  QList<QString> searchPaths;
  QMap<QString,APlugin *> plugins;

  static QString libExtension;
};

#endif
