#include "AEvent.h"

#include "AEventPackage.h"

#include <QDebug>

AEvent::AEvent()
  :numTracks(0),numChargedHadrons(0),numPhotons(0),numNeutralHadrons(0),numNeutrinos(0),numMuons(0),numElectrons(0),highestTrackID(0),package(0)
{}

void AEvent::LoadEvent() { }

void AEvent::addTrack(ATrack* track)
{
  switch(track->Type)
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
    tags.insert(tag);
  else
    tags.remove(tag);

  if(package) package->save();
}

ATrack* AEvent::getTrackById(unsigned int ID)
{
  for ( int i=0;i<Tracks.size();i++)
    {
      if ( Tracks[i]->trackID == ID ) //Found it
	{
	  return Tracks[i];
	}
    }

  return 0;
}

void AEvent::addAnalysisData(QString modulename,AEventAnalysisData* data)
{
  if(package)
    connect(data,SIGNAL(updated()),
	    package,SLOT(save()));
  
  _analysisData[modulename]=data;
}

AEventAnalysisData* AEvent::getAnalysisData(QString modulename)
{
  if(!_analysisData.contains(modulename))
    {
      addAnalysisData(modulename,new AEventAnalysisData(modulename));
      if(package) package->save(); //Save, to ensure that even empty data sets are saved
    }

  return _analysisData[modulename];
}

QList<QString> AEvent::listAnalysisData()
{
  return _analysisData.keys();
}

QList<ATrack*> AEvent::getInterestingTracks()
{
  QList<ATrack*> list;
  
  int ptMinimum = 2;
  for(int i=0;i<Tracks.size();i++)
    {
      
      ATrack* t = Tracks[i];
      if ( (t->Type == t->eJet))
      {
      list.push_back(t);
      }
      
      
      if ( (t->Type == t->eMissingEt))
        {
	  qDebug() << "getInterestingTracks() got MissingET";
	  list.push_back(t);
	  qDebug() << "Really got that MissingEt. No problem";
        }
      
      if ( t->Type == t->eSTrack )
        {
	  //cout << "\nAbout to evaluate a STrack";
	  ASTrack* goo = static_cast<ASTrack*>(t);
	  //ASTrack* goo = go->getThisSTrack();
	  //if (goo->pt >= 1) cout << "\nEvaluating a STrack. It's a " << goo->name << " of pt " << goo->pt << " with code "<< goo->code;
	  if ( goo->pt >= ptMinimum )
            {
	      
	      //                cout << "\nEvaluating a STrack. It's a " << goo->name << " of pt " << goo->pt << " with code "<< goo->code;
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
