#include "AShower.h"

AShower::AShower()
  : AEventObject("Shower",AEventObject::eShower)
  ,_energy(0),_sub(0),_layer(0)
{ }

AShower::~AShower()
{ }

void AShower::setEta(float eta)
{
  _eta=eta;
  _theta=2*atan( exp( -_eta ) );
  _et=_energy*sin(_theta);
}

void AShower::setPhi(float phi)
{
  _phi=phi;
}

void AShower::setCalorimeter(QString calorimenter)
{
  _calometer=calorimenter;
}

QString AShower::calorimeter()
{
  return _calometer;
}

void AShower::setEnergy(float energy)
{
  _energy=energy;
  _et=energy*sin(_theta);
}

float AShower::energy()
{
  return _energy;
}

void AShower::setSub(int sub)
{
  _sub=sub;
}

int AShower::sub()
{
  return _sub;
}

void AShower::setLayer(int layer)
{
  _layer=layer;
}

int AShower::layer()
{
  return _layer;
}
