#include "AJet.h"

#include <math.h>

AJet::AJet()
  : AEventObject("Jet",AEventObject::eJet),_emFraction(0)
{ }

AJet::~AJet()
{}

void AJet::setJetType(QString jetType)
{
  _jetType=jetType;
}

QString AJet::jetType()
{
  return _jetType;
}

void AJet::setEta(float eta)
{
  _eta=eta;
  _theta=2*atan( exp( -_eta ) );
}

void AJet::setPhi(float phi)
{
  _phi=phi;
}

void AJet::setEt(float et)
{
  _pt=et; // Massless jet, I guess...
  _et=et;
}

void AJet::setEMFraction(float emFraction)
{
  _emFraction=emFraction;
}

float AJet::emFraction()
{
  return _emFraction;
}
