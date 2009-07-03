#include "APoint3D.h"
#include <QDebug>

APoint3D::APoint3D(double x,double y,double z)
  :_x(x),_y(y),_z(z)
{

}

APoint3D::APoint3D(irr::core::vector3df point)
{
  _x=point.X;
  _y=point.Y;
  _z=point.Z;
}

double APoint3D::x()
{
  return _x;
}

double APoint3D::y()
{
  return _y;
}

double APoint3D::z()
{
  return _z;
}

void APoint3D::setX(double x)
{
  _x=x;
}

void APoint3D::setY(double y)
{
  _y=y;
}

void APoint3D::setZ(double z)
{
  _z=z;
}

APoint3D APoint3D::operator+(const APoint3D& p1)
{
  APoint3D result(_x+p1._x,_y+p1._y,_z+p1._z);
  return result;
}

APoint3D APoint3D::operator-(const APoint3D& p1)
{ 
  APoint3D result(_x-p1._x,_y-p1._y,_z-p1._z);
  return result;
}

APoint3D APoint3D::operator*(const double c)
{
  APoint3D result(_x*c,_y*c,_z*c);
  return result;
}
