#ifndef ATRACKCOMBINATION_H_
#define ATRACKCOMBINATION_H_

#include <QList>
#include <QString>
#include <QObject>

#include <aeventmanager/ATrack.h>
#include <aeventmanager/ASTrack.h>
#include <aeventmanager/AJet.h>

class ATrackCombination : public QObject {
  
  Q_OBJECT
 public:
  ATrackCombination();
  ~ATrackCombination();

  void addTrack(ATrack*);
  bool deleteTrack(ATrack*);

  int size();
  int getID();
  QString getName();
  void setName(QString name);

  float getNetCharge();
  float getInvariantMass();
  float InvMass;

  void fixIDs();

  ATrack* operator[](unsigned int idx);
 private:
  static unsigned int _IDS;

  QList<ATrack*> tracks;
  QString name;
  unsigned int id;

  float netCharge;

  //What exactly is this?
  ATrack* equivalentTrack; //a track created to add to the tracklist, pointing here

  void recalculate();

  float calculateInvariantMass();

};
#endif
