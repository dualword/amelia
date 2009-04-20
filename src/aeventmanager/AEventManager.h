#ifndef AEVENTMANAGER_H
#define AEVENTMANAGER_H

#include <APlugin.h>

#include <QMap>
#include <QFuture>
#include <QFutureWatcher>

#include "AEventPackage.h"

#include "AEventManagerDefines.h"

class AEVENTMANAGER_EXPORT AEventManager : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
    AEventManager ( QObject *parent=0 );
    ~AEventManager();

    void load();
    QStringList packageList();
    AEventPackage* package(QString name);

 public slots:
    void loadWorkspace();
    void handlePackageLoaded(int);

 signals:
    void packageLoaded(const QString& name);
    void packageAdded(const QString& name);

 private:
    QMap<QString,AEventPackage*> packages;

    QString loc;
    QString activePackage;

    QFuture<AEventPackage *> loadedFuture;
    QFutureWatcher<AEventPackage *> loadedFutureWatcher;
};

#endif // AEVENTMANAGER_H

