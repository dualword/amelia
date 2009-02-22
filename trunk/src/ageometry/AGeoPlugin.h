#ifndef AGEOPLUGIN_H
#define AGEOPLUGIN_H

#include <APlugin.h>
#include <AMELIA.h>

#include <QObject>
#include <QMainWindow>

class AGeoPlugin : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
    AGeoPlugin ( QObject *parent=0 );
    ~AGeoPlugin();

    void load();
    
    //template <class T>
    //T findWidget(QString name);
    QWidget* findWidget(QString name);
 private:
    QMainWindow *geoWin;
};

#endif // AGEOPLUGIN_H

