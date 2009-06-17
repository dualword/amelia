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

  void loadBlock(QDomElement blockElement);
    
  void prepare();
  void cleanup();

public slots:
  void updateFrame(int frame);
  
private:
  QMap<QString,ATourAction *> actions;
  QMap<QString,ATourAction *> lastActions;

  int _duration;

  Q_OBJECT
};

#endif //ATOURBLOCK_H_
