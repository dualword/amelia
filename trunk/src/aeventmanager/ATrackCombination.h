#ifndef ATRACKCOMBINATION_H_
#define ATRACKCOMBINATION_H_

#include <QList>
#include <QString>
#include <QObject>

#include "ATrack.h"
#include "ASTrack.h"
#include "AJet.h"

class AEVENTMANAGER_EXPORT ATrackCombination : public ATrack 
{
  
 public:
  ATrackCombination();
  ATrackCombination(const ATrackCombination&);
  ~ATrackCombination();
  
  void addTrack(ATrack*);
  bool deleteTrack(ATrack*);

  QString name();
  QString name(bool generateDefault);
  
  int size();

  float getInvariantMass();
  float InvMass;

  ATrack* operator[](unsigned int idx);
  ATrack* getTrack(unsigned int idx);

  bool operator==(ATrackCombination& other);
 private:
  static unsigned int _IDS;

  QList<ATrack*> tracks;

  void recalculate();

  float calculateInvariantMass();

  Q_OBJECT;
};
#endif
