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
  bool showChargedHadrons();
  bool showNeutralHadrons();
  bool showPhotons();
  bool showJets();
  bool showMissingEt();

 public slots:
  void setShowElectrons(bool);
  void setShowMuons(bool);
  void setShowChargedHadrons(bool);
  void setShowNeutralHadrons(bool);
  void setShowPhotons(bool);
  void setShowJets(bool);
  void setShowMissingEt(bool);

 signals:
  void showElectronsChanged(bool);
  void showMuonsChanged(bool);
  void showChargedHadronsChanged(bool);
  void showNeutralHadronsChanged(bool);
  void showPhotonsChanged(bool);
  void showJetsChanged(bool);
  void showMissingEtChanged(bool);
  
 private:
  bool _showElectrons;
  bool _showMuons;
  bool _showChargedHadrons;
  bool _showNeutralHadrons;
  bool _showPhotons;
  bool _showJets;
  bool _showMissingEt;
  
  Q_OBJECT
};

#endif //APARTICLEFILTER_H_
