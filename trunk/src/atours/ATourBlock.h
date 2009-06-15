#ifndef ATOURBLOCK_H_
#define ATOURBLOCK_H_

#include <QString>
#include <QList>
#include <QObject>

#include <ageometry/APoint3D.h>

#include "ATourAction.h"

class ATourBlock : public QObject
{
public:
  ATourBlock();

  void addAction(ATourAction *act);
  
  int duration();
  void reset();

  void cleanup();

public slots:
  void updateFrame(int frame);
  
private:
  
  QList<ATourAction *> act;
  QList<int>           actt;

  int _duration;

  ATourAction *lastAction;

  Q_OBJECT
};

#endif //ATOURBLOCK_H_
