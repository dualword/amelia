#ifndef AEVENT_H_
#define AEVENT_H_ 1

#include <QObject>
#include <QList>
#include <QSet>
#include <QMultiMap>

#include "AEventObject.h"
#include "AShower.h"
#include "ASTrack.h"
#include "ARTrack.h"
#include "AJet.h"
#include "AMisET.h"
#include "AFCALShower.h"
#include "ATrackFilter.h"

#include "AEventAnalysisData.h"

#include "AEventManagerDefines.h"

class AEventPackage;

class AEVENTMANAGER_EXPORT AEvent : public QObject
{
 public:
  AEvent();
  ~AEvent() {};

  //Data
  int runNumber;
  int eventNumber;

  QList<AEventObject*> Tracks; // Tracks pointers are stored here

  QList<ASTrack*> STracks; // Pointers to simulated tracks within "EventTracks" are stored here
  QList<ARTrack*> RTracks; // Pointers to reconstructed tracks within "EventTracks" are stored here
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
  int numJets;

  QList<AShower*> LArshowers;
  QList<AFCALShower*> FCALshowers;
  QList<AShower*> HECshowers;
  QList<AShower*> TILEshowers;
  float ETMis;
  float CaloETMis;

  virtual void LoadEvent();

  void setName(QString);
  QString name();

  QString uid();

  void tag(QString tag,bool state);
  QSet<QString> tags();

  AEventPackage *package();
  virtual void setPackage(AEventPackage *package);

  QList<QString> availableJetTypes();
  QList<QString> availableMisETTypes();
  QList<QString> availableTrackTypes();

  AEventObject* getTrackById(unsigned int id);

  template <class T>
    T* getAnalysisData(QString module)
  {
    QList<AEventAnalysisData*> data=_analysisData.values(module);
    for(int i=0;i<data.size();i++)
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

 signals:
  void modified();

 protected:
  int highestTrackID;

  void addTrack(AEventObject* track);
  void addTrack(ASTrack* track);
  void addTrack(ARTrack* track);
  void addTrack(AJet* track);
  void addTrack(AMisET* track);
  void addTrack(AShower* track);
  void addTrack(AFCALShower* track);

 private:
  QMultiMap<QString,AEventAnalysisData*> _analysisData;

  AEventPackage *_package;
  QSet<QString> _tags;
  QList<QString> _jetTypes;
  QList<QString> _misETTypes;
  QList<QString> _trackTypes;
  QString _name;

  Q_OBJECT
};

#endif //AEVENT_H_
