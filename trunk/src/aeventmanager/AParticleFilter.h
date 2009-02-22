#ifndef APARTICLEFILTER_H_
#define APARTICLEFILTER_H_

#include "ATrackFilter.h"

#include "ASTrack.h"

class AParticleFilter : public ATrackFilter
{
  Q_OBJECT
    public:
  AParticleFilter(ATrackFilter *next);

  bool checkTrack(ATrack *);

  public slots:
  void setShowElectrons(bool);
  void setShowMuons(bool);
  void setShowChargedHadrons(bool);
  void setShowNeutralHadrons(bool);
  void setShowPhotons(bool);
  void setShowJets(bool);
  void setShowMissingEt(bool);

  private:
  bool _showElectrons;
  bool _showMuons;
  bool _showChargedHadrons;
  bool _showNeutralHadrons;
  bool _showPhotons;
  bool _showJets;
  bool _showMissingEt;
};

#endif //APARTICLEFILTER_H_
