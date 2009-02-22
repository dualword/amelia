#ifndef ASTRACK_H_INCLUDED
#define ASTRACK_H_INCLUDED

#include <QString>

#include "AEventManagerDefines.h"

/// A generic "track" of the event. Types: 0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
class AEVENTMANAGER_EXPORT ATrack
{

 public:
  enum eType //element type
  {
    eUndefined,
    eSTrack,
    eJet,
    eShower,
    eFCALShower,
    eMissingEt,
    eCombination
  };
  static unsigned int highestTrackID;

  ATrack();
  ATrack(QString name);

  eType Type;

  QString name;
  int trackID;
  int selectionID;

  bool isInList;

  int q;
  float pt;

 private:
};

#endif // ASTRACK_H_INCLUDED
