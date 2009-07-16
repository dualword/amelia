#include "AEvent.h"

#include <aeventmanager/AEventPackage.h>

#include <QDebug>
#include <QStringList>

AEvent::AEvent()
  :numTracks(0),numChargedHadrons(0),numPhotons(0),numNeutralHadrons(0),numNeutrinos(0),numMuons(0),numElectrons(0),numJets(0),
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

void AEvent::addTrack(ATrack* track)
{
  switch(track->type())
    {
    case ATrack::eSTrack:
      addTrack((ASTrack*)track);
      break;
    case ATrack::eJet:
      addTrack((AJet*)track);
      break;
    case ATrack::eMissingEt:
      addTrack((AMisET*)track);
      break;
    case ATrack::eShower:
      addTrack((AShower*)track);
      break;
    default:
      qDebug() << "Adding unsupported track...";
      break;
    }
}

void AEvent::addTrack(ASTrack* track)
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

  STracks.push_back(track);
  numTracks++;
  Tracks.push_back(track);

}

void AEvent::addTrack(AJet* jet)
{
  Jets.push_back(jet);
  numTracks++;
  numJets++;
  Tracks.push_back(jet);

}

void AEvent::addTrack(AMisET* miset)
{
  MisET.push_back(miset);
  numTracks++;
  Tracks.push_back(miset);
}

void AEvent::addTrack(AShower* shower)
{
  if(shower->calometer=="LAr")
    LArshowers.push_back(shower);
  else if(shower->calometer=="HEC")
    HECshowers.push_back(shower);
  else if(shower->calometer=="TILE")
    TILEshowers.push_back(shower);
	
}

void AEvent::addTrack(AFCALShower* shower)
{
  FCALshowers.push_back(shower);
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

ATrack* AEvent::getTrackById(unsigned int ID)
{
  for ( int i=0;i<Tracks.size();i++)
    {
      if ( Tracks[i]->trackID() == ID ) //Found it
	{
	  return Tracks[i];
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

QList<ATrack*> AEvent::getInterestingTracks()
{
  QList<ATrack*> list;
  
  int ptMinimum = 2;
  for(int i=0;i<Tracks.size();i++)
    {
      
      ATrack* t = Tracks[i];
      if ( (t->type() == ATrack::eJet))
      {
      list.push_back(t);
      }
      
      
      if ( (t->type() == ATrack::eMissingEt))
        {
	  list.push_back(t);
        }
      
      if ( t->type() == ATrack::eSTrack )
        {
	  //cout << "\nAbout to evaluate a STrack";
	  ASTrack* goo = static_cast<ASTrack*>(t);
	  //ASTrack* goo = go->getThisSTrack();
	  //if (goo->pt >= 1) cout << "\nEvaluating a STrack. It's a " << goo->name << " of pt " << goo->pt << " with code "<< goo->code;
	  if ( goo->Pt() >= ptMinimum )
            {
	      if ( goo->isElectron() ) //electrons
                {
		  qDebug() << "getInterestingTracks() got an electron";
		  list.push_back(t);
                }
	      
	      if ( goo->isMuon() ) //muons
                {
		  qDebug() << "getInterestingTracks() got a muon";
		  list.push_back(t);
                }
	      
	      if ( goo->isPhoton() ) //photons
                {
		  qDebug() << "getInterestingTracks() got a photon";
		  list.push_back(t);
                }
            }
        }//eSTrack
    }

  return list;
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
