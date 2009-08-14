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

  QList<AEventObject*> Objects; // Tracks pointers are stored here

  QList<ATrack*> Tracks; // Pointers to simulated tracks within "EventTracks" are stored here
  QList<AJet*> Jets;  // Pointers to Jets within "EventTracks" are stored here
  QList<AMisET*> MisET;  // Pointers to the missing energy withing this event.

  // Hash showers by id, to provide fast lookup
  QHash<int,AShower*> LArshowers;
  QHash<int,AShower*> HECshowers;
  QHash<int,AShower*> TILEshowers;
  QHash<int,AFCALShower*> FCALshowers;

  int numChargedHadrons;
  int numPhotons;
  int numNeutralHadrons;
  int numNeutrinos;
  int numMuons;
  int numElectrons;
  int numTracks;

  int numShowers;
  int numJets;

  int numParticles;

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

  AEventObject* getObjectById(unsigned int id);

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

  void addObject(AEventObject* object);
  void addTrack(ATrack* track);
  void addJet(AJet* track);
  void addMisET(AMisET* track);
  void addShower(AShower* track);
  void addFCALShower(AFCALShower* track);

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
