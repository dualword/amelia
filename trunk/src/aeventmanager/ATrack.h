#ifndef ATRACK_H_
#define ATRACK_H_

#include <QString>
#include <QObject>
#include <irrlicht.h>
#include <vector>


#include "AEventManagerDefines.h"

/// A generic "track" of the event. Types: 0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
class AEVENTMANAGER_EXPORT ATrack : public QObject
{
 public:
  enum eType //element type
  {
    eUndefined,
    eSTrack,
    eRTrack,
    eJet,
    eShower,
    eFCALShower,
    eMissingEt,
    eCombination
  };
  static unsigned int highestTrackID;

  ATrack();
  ATrack(QString name,ATrack::eType);
  ATrack(const ATrack&);

  eType type();

  unsigned int trackID();
  void setTrackID(unsigned int);

  unsigned int selectionID();
  void setSelectionID(unsigned int);

  int charge();
  void setCharge(int);

  virtual QString name();
  void setName(QString);

  float Pt();
  void setPt(float);

  virtual bool isInteresting();
  void setInteresting(bool);

 signals:
  void updated();

 private:
  eType _type;

  QString _name;
  unsigned int _trackID;
  int _selectionID;

  int _charge;
  float _pt;

  bool _interesting;

  Q_OBJECT
};

#endif // ATRACK_H_
