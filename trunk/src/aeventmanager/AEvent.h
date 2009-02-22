#ifndef AEVENT_H_
#define AEVENT_H_ 1

#include <QObject>
#include <QList>
#include <QSet>

#include <AVector.h>

#include "ATrack.h"
#include "AShower.h"
#include "ASTrack.h"
#include "AJet.h"
#include "AMisET.h"
#include "AFCALShower.h"
#include "ATrackFilter.h"

#include "AEventManagerDefines.h"

class AEVENTMANAGER_EXPORT AEventPackage;

class AEVENTMANAGER_EXPORT AEvent : public QObject
{
  Q_OBJECT
  public:
  AEvent();
  ~AEvent() {};
  QString filename;
  QString location;
  bool read;
  QSet<QString> tags;
  AEventPackage *package;

  //Data
  int runNumber;
  int eventNumber;

  QList<ATrack*> Tracks; // Tracks pointers are stored here

  QList<ASTrack*> STracks; // Pointers to simulated tracks within "EventTracks" are stored here
  QList<AJet*> Jets;  // Pointers to Jets within "EventTracks" are stored here
  QList<AMisET*> MisET;  // Pointers to Jets within "EventTracks" are stored here

  int numTracks;
  int numChargedHadrons;
  int numPhotons;
  int numNeutralHadrons;
  int numNeutrinos;
  int numMuons;
  int numElectrons;
  int numShowers;

  QList<AShower*> LArshowers;
  QList<AFCALShower*> FCALshowers;
  QList<AShower*> HECshowers;
  QList<AShower*> TILEshowers;
  float ETMis;
  AVector<float> ETMisVec;
  float CaloETMis;
  AVector<float> CaloETMisVec;

  virtual void LoadEvent();

  void markAsRead(bool);
  void tag(QString,bool);

  void addTrack(ATrack* track);
  void addTrack(ASTrack* track);
  void addTrack(AJet* track);
  void addTrack(AMisET* track);
  void addTrack(AShower* track);
  void addTrack(AFCALShower* track);

  QList<ATrack*> getInterestingTracks();

 protected:
  int highestTrackID;
};

#endif //AEVENT_H_
