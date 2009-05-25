#ifndef AEVENT_H_
#define AEVENT_H_ 1

#include <QObject>
#include <QList>
#include <QSet>
#include <QMultiMap>

#include "ATrack.h"
#include "AShower.h"
#include "ASTrack.h"
#include "AJet.h"
#include "AMisET.h"
#include "AFCALShower.h"
#include "ATrackFilter.h"

#include "AEventAnalysisData.h"

#include "AEventManagerDefines.h"

class AEventPackage;

class AEVENTMANAGER_EXPORT AEvent : public QObject
{
  Q_OBJECT
  public:
  AEvent();
  ~AEvent() {};
  QString filename;
  QString location;
  QSet<QString> tags;

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
  float CaloETMis;

  virtual void LoadEvent();

  void tag(QString,bool);

  AEventPackage *package();
  virtual void setPackage(AEventPackage *package);

  ATrack* getTrackById(unsigned int id);

  template <class T>
   T* getAnalysisData(QString module)
  {
    QList<AEventAnalysisData*> data=_analysisData.values(module);
    for(unsigned int i=0;i<data.size();i++)
      {
	T* ret=qobject_cast<T*>(data[i]);
	if(ret!=0)
	  return ret;
      }
    
    //Nothing found, create new one
    T* ret=new T(module);
    addAnalysisData(module,ret);
    return ret;
  }
  
  void addAnalysisData(QString module,AEventAnalysisData* data);
  QList<QString> listAnalysisData();
  QList<AEventAnalysisData*> allAnalysisData();

  QList<ATrack*> getInterestingTracks();

 protected:
  int highestTrackID;

  void addTrack(ATrack* track);
  void addTrack(ASTrack* track);
  void addTrack(AJet* track);
  void addTrack(AMisET* track);
  void addTrack(AShower* track);
  void addTrack(AFCALShower* track);

 private:
  QMultiMap<QString,AEventAnalysisData*> _analysisData;

  AEventPackage *_package;
};

#endif //AEVENT_H_
