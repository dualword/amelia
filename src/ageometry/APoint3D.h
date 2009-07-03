#ifndef APOINT3D_H_
#define APOINT3D_H_

#include <irrlicht.h>

#include "AGeometryDefines.h"
class AGEOMETRY_EXPORT APoint3D
{
 public:
  APoint3D(double x=0,double y=0,double z=0);
  APoint3D(irr::core::vector3df point);
  
  double x();
  void setX(double x);

  double y();
  void setY(double y);

  double z();  
  void setZ(double z);

  APoint3D operator+(const APoint3D& p1);
  APoint3D operator-(const APoint3D& p1);
  APoint3D operator*(const double c);

 private:
  double _x,_y,_z;
};

#endif //APOINT3D_H_
