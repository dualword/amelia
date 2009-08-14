#include "AEvent.h"

#include <aeventmanager/AEventPackage.h>

#include <QDebug>
#include <QStringList>

AEvent::AEvent()
  :runNumber(-1),eventNumber(-1),
   numParticles(0),
   numChargedHadrons(0),numPhotons(0),numNeutralHadrons(0),numNeutrinos(0),numMuons(0),numElectrons(0),numJets(0),
   highestTrackID(0),_package(0)
{}

void AEvent::LoadEvent() { }

void AEvent::setName(QString name)
{
  _name=name;
}

QString AEvent::name()
{
  return _name;
}

QString AEvent::uid()
{
  return QString::number(runNumber)+":"+QString::number(eventNumber);
}

QList<QString> AEvent::availableJetTypes()
{
  return _jetTypes;
}

QList<QString> AEvent::availableMisETTypes()
{
  return _misETTypes;
}

QList<QString> AEvent::availableTrackTypes()
{
  return _trackTypes;
}

void AEvent::addObject(AEventObject* object)
{
  switch(object->type())
    {
    case AEventObject::eTrack:
      addTrack((ATrack*)object);
      break;
    case AEventObject::eJet:
      addJet((AJet*)object);
      break;
    case AEventObject::eMissingEt:
      addMisET((AMisET*)object);
      break;
    case AEventObject::eShower:
      addShower((AShower*)object);
      break;
    case AEventObject::eFCALShower:
      addFCALShower((AFCALShower*)object);
      break;
    case AEventObject::eUndefined:
      qDebug() << "Adding undefined object...";
      break;
    default:
      qDebug() << "Adding unknown object...";
      break;
    }
}

void AEvent::addTrack(ATrack* track)
{
  if(track->isElectron())
    numElectrons++;
  else if(track->isMuon())
    numMuons++;
  else if(track->isPhoton())
    numPhotons++;
  else if(track->isNeutrino())
    numNeutrinos++;
  else if(track->isNeutralHadron())
    numNeutralHadrons++;
  else if(track->isChargedHadron())
    numChargedHadrons++;

  numParticles++;
  numTracks++;
  
  QString ttype=track->trackType();
  if(!_trackTypes.contains(ttype))
    _trackTypes.push_back(ttype);

  Tracks.push_back(track);
  Objects.push_back(track);
}


void AEvent::addJet(AJet* jet)
{
  QString jtype=jet->jetType();
  if(!_jetTypes.contains(jtype))
    _jetTypes.push_back(jtype);
  
  Jets.push_back(jet);
  numParticles++;
  numJets++;
  Objects.push_back(jet);

}

void AEvent::addMisET(AMisET* miset)
{
  QString mtype=miset->misETType();
  if(!_misETTypes.contains(mtype))
    _misETTypes.push_back(mtype);

  MisET.push_back(miset);
  Objects.push_back(miset);
}

void AEvent::addShower(AShower* shower)
{
  if(shower->calorimeter()=="LAr")
    LArshowers.insert(shower->ID(),shower);
  else if(shower->calorimeter()=="HEC")
    HECshowers.insert(shower->ID(),shower);
  else if(shower->calorimeter()=="TILE")
    TILEshowers.insert(shower->ID(),shower);

  Objects.push_back(shower);
}

void AEvent::addFCALShower(AFCALShower* shower)
{
  FCALshowers.insert(shower->ID(),shower);
}

void AEvent::tag(QString tag, bool status)
{
  if(status)
    _tags.insert(tag);
  else
    _tags.remove(tag);

  emit modified();
}

QSet<QString> AEvent::tags()
{
  return _tags;
}

AEventObject* AEvent::getObjectById(unsigned int ID)
{
  for ( int i=0;i<Objects.size();i++)
    {
      if ( Objects[i]->ID() == ID ) //Found it
	{
	  return Objects[i];
	}
    }

  return 0;
}

void AEvent::addAnalysisData(QString modulename,AEventAnalysisData* data)
{
  if(_package)
    connect(data,SIGNAL(updated()),
	    _package,SLOT(save()));

  _analysisData.insert(modulename,data);
}

QList<QString> AEvent::listAnalysisData()
{
  return _analysisData.keys();
}

QList<AEventAnalysisData*> AEvent::allAnalysisData()
{
  return _analysisData.values();
}

AEventPackage* AEvent::package()
{
  return _package;
}

void AEvent::setPackage(AEventPackage *pkg)
{
  AEventPackage *tmpPkg=_package;
  _package=0;
  if(tmpPkg==pkg)
    {
      disconnect(this,SIGNAL(modified()),
		 tmpPkg,SLOT(save()));
	return;
    }
  if(tmpPkg!=0) tmpPkg->removeEvent(this);
  _package=pkg;

  connect(this,SIGNAL(modified()),
	  pkg,SLOT(save()));
}
