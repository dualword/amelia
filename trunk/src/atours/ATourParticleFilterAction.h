#ifndef ATOURPARTICLEFILTERACTION_H_
#define ATOURPARTICLEFILTERACTION_H_

#include "ATourAction.h"

#include <aeventmanager/AParticleFilter.h>

class ATourParticleFilterAction : public ATourAction
{
public:
  Q_INVOKABLE ATourParticleFilterAction();
  Q_INVOKABLE ATourParticleFilterAction(QString particle);

  QString type();

  void loadFromXML(QDomElement actionElement);
  
  void act();

  void prepare();

private:
  AParticleFilter *filter;

  QString particle;
  bool state;

  Q_OBJECT
};

#endif //ATOURPARTICLEFILTERACTION_H_
