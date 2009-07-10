#ifndef APARTICLEFILTER_H_
#define APARTICLEFILTER_H_

#include "ATrackFilter.h"

#include "ASTrack.h"

#include <libamelia/QBoolSync.h>

class AEVENTMANAGER_EXPORT AParticleFilter : public ATrackFilter
{
 public:
  AParticleFilter(ATrackFilter *next=0);
  
  bool checkTrack(ATrack *);

  bool showElectrons();
  bool showMuons();
  bool showHadrons();
  bool showPhotons();
  bool showJets();
  bool showMissingEt();

 public slots:
  void setShowElectrons(bool);
  void setShowMuons(bool);
  void setShowHadrons(bool);
  void setShowPhotons(bool);
  void setShowJets(bool);
  void setShowMissingEt(bool);

 signals:
  void showElectronsChanged(bool);
  void showMuonsChanged(bool);
  void showHadronsChanged(bool);
  void showPhotonsChanged(bool);
  void showJetsChanged(bool);
  void showMissingEtChanged(bool);
  
 private:
  bool _showElectrons;
  bool _showMuons;
  bool _showHadrons;
  bool _showPhotons;
  bool _showJets;
  bool _showMissingEt;
  
  Q_OBJECT
};

#endif //APARTICLEFILTER_H_
