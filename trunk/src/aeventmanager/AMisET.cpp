#include "AMisET.h"

AMisET::AMisET():ATrack()
{
  name="MisET";
  Type=eMissingEt;
}

void AMisET::setMisETType(mType type)
{
  MisETType=type;
}

void AMisET::setMisETType(QString type)
{
  mType mtype;
  if (type == QString("MET_Final"))
    {
      mtype = AMisET::mMET_Final;
    }
  if (type == QString("MET_RefMuon"))
    {
      mtype = AMisET::mMET_RefMuon;
    }
  if (type == QString("MET_Calib"))
    {
      mtype = AMisET::mMET_Calib;
    }
  if (type == QString("MET_RefFinal"))
    {
      mtype = AMisET::mMET_RefFinal;
    }
  if (type == QString("MET_Truth"))
    {
      mtype = AMisET::mMET_Truth;
    }
  setMisETType(mtype);
}
