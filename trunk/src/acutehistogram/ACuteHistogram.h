#ifndef ACUTEHISTOGRAM_H
#define ACUTEHISTOGRAM_H

#include <APlugin.h>

#include <aeventmanager/AFilteredEvent.h>

#include "AHistogramWidget.h"

class ACuteHistogram : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
  ACuteHistogram ( QObject *parent=0 );
  ~ACuteHistogram();
  
  void load();

public slots:
  void handleNewEventLoaded(AFilteredEvent *filteredevent);
  void updateHistogram();

private:
  AFilteredEvent *_event;

  AHistogramWidget *_histWdg;
};

#endif // ACUTEHISTOGRAM_H

