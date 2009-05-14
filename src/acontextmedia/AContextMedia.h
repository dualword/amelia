#ifndef ACONTEXTMEDIA_H
#define ACONTEXTMEDIA_H

#include <APlugin.h>

#include <QMap>
#include <QVBoxLayout>

#include <ageometry/AGeoPlugin.h>

#ifdef QT_PHONON_LIB
#include <Phonon>
#else
#include <Phonon/VideoPlayer>
#endif


class AContextMedia : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
  AContextMedia ( QObject *parent=0 );
  ~AContextMedia();
  
  void load();
  
public slots:
  void handleDetectorSelected(const QString&);
  void playVideo(const QString&);

private:
  QMap<QString,QString> headers,text;

  QAction *stopAction;
  QSignalMapper videoMapper;
  AGeoPlugin* geo;
};

#endif // ACONTEXTMEDIA_H

