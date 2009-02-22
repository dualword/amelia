#include "AJet.h"

#include <math.h>

AJet::AJet():ATrack()
{
  Type=ATrack::eJet;
  name="Jet";
}

void AJet::setJetType(jType type)
{
  JetType=type;
}

void AJet::setJetType(QString type)
{
  jType jtype;
  if (type == QString("Kt4H1TopoJets"))
    {
      jtype = AJet::jKt4H1TopoJets;
    }
  if (type == QString("Cone4H1TopoJets"))
    {
      jtype = AJet::jCone4H1TopoJets;
    }
  if (type == QString("Kt4H1TowerJets"))
    {
      jtype = AJet::jKt4H1TowerJets;
    }
  if (type == QString("Cone4H1TowerJets"))
    {
      jtype = AJet::jCone4H1TowerJets;
    }

  setJetType(jtype);
}

float AJet::getTl()
{
  return 0.5 * ( exp (eta) - exp (-eta));
}
