#ifndef ATOURCAMERAACTION_H_
#define ATOURCAMERAACTION_H_

#include "ATourAction.h"

#include <ageometry/APoint3D.h>
#include <ageometry/AGeometry.h>

#include <AMELIA.h>

class ATourCameraAction : public ATourAction
{
public:
  Q_INVOKABLE ATourCameraAction();

  void loadFromXML(QDomElement actionElement);
  
  void doAction();
  void updateAction(double done);

private:
  APoint3D position;
  APoint3D target;

  APoint3D positionInitial;
  APoint3D targetInitial;

  AGeometry *geo;

  APoint3D interpolate(APoint3D start,APoint3D end,double time);

  Q_OBJECT
};

#endif //ATOURCAMERAACTION_H_
