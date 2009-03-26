#ifndef AMISET_H_
#define AMISET_H_

#include "ATrack.h"
#include "AEventManagerDefines.h"

//Objects of this class represent Missing Energy
class AMisET : public ATrack
{
 public:
  enum mType // type
  {
        mMET_Final,
        mMET_RefMuon,
        mMET_Calib,
        mMET_RefFinal,
        mMET_Truth
  };


  AMisET();
  ~AMisET();

  float etx;
  float ety;
  float et;

  mType MisETType;

  void setMisETType(mType type);
  void setMisETType(QString type);

};
#endif //AMISET_H_
