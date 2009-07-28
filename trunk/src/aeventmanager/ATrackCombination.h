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

  QString trackIDString();
  QString name();
  QString name(bool generateDefault);
  
  int size();

  float getInvariantMass();

  ATrack* operator[](unsigned int idx);
  ATrack* getTrack(unsigned int idx);

  bool operator==(ATrackCombination& other);

 public slots:
  void handleTrackUpdated();

 private:
  QList<ATrack*> tracks;

  double Px;
  double Py;
  double Pz;
  double E;
  
  float InvMass;

  void recalculate();
  void recalculate(ATrack *trk);

  float calculateInvariantMass();

  Q_OBJECT;
};
#endif
