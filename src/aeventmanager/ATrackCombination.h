#ifndef ATRACKCOMBINATION_H_
#define ATRACKCOMBINATION_H_

#include <QList>
#include <QString>
#include <QObject>

#include "AEventObject.h"
#include "ASTrack.h"
#include "AJet.h"

class AEVENTMANAGER_EXPORT ATrackCombination : public AEventObject 
{
  
 public:
  ATrackCombination();
  ATrackCombination(const ATrackCombination&);
  ~ATrackCombination();
  
  void addTrack(AEventObject*);
  bool deleteTrack(AEventObject*);

  QString trackIDString();
  QString name();
  QString name(bool generateDefault);
  
  int size();

  float getInvariantMass();

  AEventObject* operator[](unsigned int idx);
  AEventObject* getTrack(unsigned int idx);

  bool operator==(ATrackCombination& other);

 public slots:
  void handleTrackUpdated();

 private:
  QList<AEventObject*> tracks;

  double Px;
  double Py;
  double Pz;
  double E;
  
  float InvMass;

  void recalculate();
  void recalculate(AEventObject *trk);

  float calculateInvariantMass();

  Q_OBJECT;
};
#endif
