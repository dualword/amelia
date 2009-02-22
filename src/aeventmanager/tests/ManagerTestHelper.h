#ifndef MANAGERTESTHELPER_H_
#define MANAGERTESTHELPER_H_

#include <QObject>

#include <aeventmanager/AEvent.h>
#include <aeventmanager/AEventManagerScene.h>

#include "AShittyHistogram.h"

class ManagerTestHelper : public QObject
{
 Q_OBJECT

 public:
  ManagerTestHelper(AShittyHistogram *hist,AEventManagerScene *model,QObject *parent=0);
  
 public slots:
  void handleNewEvent(AEvent *);

 private:
  AShittyHistogram *hist;
  AEventManagerScene *model;
};

#endif //MANAGERTESTHELPER_H_
