#include "ASelectionInfoScene.h"

#include <QInputDialog>
#include "AGeometry.h"
#include "ATrackTableModel.h"

ASelectionInfoScene::ASelectionInfoScene(QObject* parent):QGraphicsScene(parent)
{
  combo=new ATrackCombination;
  analysisData=new ATrackCollection("AGeometry");
  init();
}

ASelectionInfoScene::~ASelectionInfoScene()
{
  delete combo;
}

void ASelectionInfoScene::init()
{
    QFont serifFont("Times", 10, QFont::Bold);

    // Info text
    header=addText("SELECTED TRACK INFO",serifFont);
    header->setPos(0,0);
    header->setDefaultTextColor("red");
    name=addText("Name:");
    name->setPos(0,25);
    name->setDefaultTextColor("white");
    charge=addText("Charge:");
    charge->setPos(0,40);
    charge->setDefaultTextColor("white");

    invmass=addText("Inv Mass:");
    invmass->setPos(0,55);
    invmass->setDefaultTextColor("white");

    id=addText("ID");
    id->setPos(0,55);
    id->setDefaultTextColor("white");

    pt=addText("Pt:");
    pt->setPos(130,25);
    pt->setDefaultTextColor("white");

    // Buttons
    QPushButton *_addTrack=new QPushButton("Bookmark Selected");
    addTrack=addWidget(_addTrack);
    addTrack->setVisible(true);
    addTrack->setPos(220,20);
    connect(_addTrack,SIGNAL(clicked()), 
	    this,SLOT(handleAddTrack()));

    QPushButton *_combTrack=new QPushButton("Combine Tracks");
    combTrack=addWidget(_combTrack);
    combTrack->setVisible(true);
    combTrack->setPos(220,50);
    connect(_combTrack,SIGNAL(clicked()),
	    this,SLOT(handleCombTracks()));

    // Main message
    serifFont.setPointSize(14);
    message=addText("",serifFont);
    message->setPos(20,20);
    message->setVisible(false);

    icon=addPixmap(QPixmap());
    icon->setPos(10,20);


    // Hide Everything by Default
    QList<QGraphicsItem*> _items=items();
    for (int i=0;i<_items.size();i++)
    {
      _items[i]->setVisible(false);
    }
}

void ASelectionInfoScene::handleAddTrack()
{
  QList<ATrack *> tracks=analysisData->getCollection("bookmarked_tracks");

  for (int i=0;i<combo->size();i++)
    {
      ATrack *strack=(*combo)[i];
      if (strack->selectionID()==0)
	strack->setSelectionID(++ATrackTableModel::selectionID);
      
      if (tracks.indexOf(strack)<0) //Dupe Check
	{
	  qDebug() << "Adding track for " << strack->name();
	  tracks.append(strack);
	}
    }
  analysisData->setCollection("bookmarked_tracks",tracks);
}

void ASelectionInfoScene::handleCombTracks()
{
  QList<ATrack *> tracks=analysisData->getCollection("combined_tracks");
  tracks.append(new ATrackCombination(*combo));
  analysisData->setCollection("combined_tracks",tracks);
}

void ASelectionInfoScene::handleViewportChange(int from,int to)
{
  switch (to)
    {
    case AGeometry::Cam3D:
      hideMessage();
      break;
    case AGeometry::Front:
    case AGeometry::Side:
      displayMessage("Track selection only supported<br/> in first-person camera mode.");
      break;
    }
}

void ASelectionInfoScene::handleNewEventLoaded(AEvent *newEvent)
{
  disconnect(analysisData,SIGNAL(updated()),
	     this,SLOT(refresh()));

  if(newEvent)
    {
      analysisData=newEvent->getAnalysisData<ATrackCollection>("AGeometry");
      connect(analysisData,SIGNAL(updated()),
	      this,SLOT(refresh()));
    }
}

void ASelectionInfoScene::displayMessage(QString text,QString headerText,QPixmap img)
{
  //TODO: Automatic line breaks and centering using font metrics
  message->setVisible(true);

  if(!img.isNull())
    {
      int imgWidth=img.width();
      message->setPos(25+imgWidth,15);
      message->setTextWidth(350-imgWidth-5);
      icon->setVisible(true);      
      icon->setPixmap(img);
    }
  else
    {
      message->setPos(20,20);
      message->setTextWidth(350);
      icon->setVisible(false);
    }
  
  if(!headerText.isEmpty())
    {
      message->setHtml(text);
      header->setPlainText(headerText);
      header->setVisible(true);
    }
  else
    {
      message->setHtml("<center>"+text+"</center>");
      header->setVisible(false);
    }

  //Hide track information
  name->setVisible(false);
  charge->setVisible(false);
  invmass->setVisible(false);
  pt->setVisible(false);
  id->setVisible(false);
  addTrack->setVisible(false);
  combTrack->setVisible(false);
}

void ASelectionInfoScene::hideMessage()
{
  // Only hide if a message is displayed
  // This makes sure that we don't hide
  // The header during track selection..
  // TODO: Restore any previous display, ei track selection
  if(message->isVisible())
    {
      message->setVisible(false);
      icon->setVisible(false);
      header->setVisible(false);
    }
}

void ASelectionInfoScene::updateTrackInfo ( ATrack* strack )
{
  message->setVisible(false);
  
  combo->addTrack(strack);
  refresh();
}

void ASelectionInfoScene::removeTrackInfo(ATrack* strack)
{
  if (combo->deleteTrack(strack))
    refresh();
}

void ASelectionInfoScene::refresh()
{
  //Ok, idea here is to look at what we got and only display needed stuff
  
  //If it all empty, hide everything
  if (combo->size()==0)
    {
      header->setVisible(false);
      name->setVisible(false);
      charge->setVisible(false);
      invmass->setVisible(false);
      pt->setVisible(false);
      id->setVisible(false);
      addTrack->setVisible(false);
      combTrack->setVisible(false);
      message->setVisible(false);
      icon->setVisible(false);
      return; //Dooone;
    }
  else if (combo->size()==1)
    //Only 1 thing, we can handle that
    {
      if ( (*combo)[0]->type() == ATrack::eSTrack ) //track
        {
	  ASTrack* STrack = static_cast<ASTrack*>((*combo)[0]);
	  header->setPlainText("SELECTED TRACK INFO");
	  name->setHtml("<b>Name:</b> "+QString(STrack->name()));
	  charge->setHtml("<b>Charge:</b> "+QString::number(STrack->charge()));
	  pt->setHtml("<b>Pt:</b> "+QString::number(STrack->Pt()));
	  id->setHtml("<b>id:</b> "+QString::number(STrack->trackID()));
	  
	  
	  header->setVisible(true);
	  name->setVisible(true);
	  charge->setVisible(true);
	  invmass->setVisible(false);
	  pt->setVisible(true);
	  id->setVisible(true);
	  combTrack->setVisible(false);
	  icon->setVisible(false);
        }
      else if ( (*combo)[0]->type() == ATrack::eJet ) //jet
        {
	  AJet* Jet = static_cast<AJet*>((*combo)[0]);
	  header->setPlainText("SELECTED JET INFO");
	  name->setHtml("<b>Type:</b> Jet");
	  pt->setHtml("<b>Pt:</b> "+QString::number(Jet->et));
	  
	  header->setVisible(true);
	  name->setVisible(true);
	  charge->setVisible(false);
	  invmass->setVisible(false);
	  pt->setVisible(true);
	  combTrack->setVisible(false);
	  icon->setVisible(false);
        }
      else if ( (*combo)[0]->type() == ATrack::eMissingEt ) //Missing Et
        {
	  AMisET* ET = static_cast<AMisET*>((*combo)[0]);
	  header->setPlainText("SELECTED MisEt INFO");
	  name->setHtml("<b>Type:</b> Missing Et");
	  pt->setHtml("<b>Et:</b> "+QString::number(ET->et));
	  
	  header->setVisible(true);
	  name->setVisible(true);
	  charge->setVisible(false);
	  invmass->setVisible(false);
	  pt->setVisible(true);
	  combTrack->setVisible(false);
	  icon->setVisible(false);
        }
    }
  else
    //Multi-track selection
    {

      // Show the combo button if more than one track is selected..
      QList<ATrack*> combines=analysisData->getCollection("combined_tracks");
      QString combName="Unknown Combination";
      combTrack->setVisible(true);
      emit combineButtonEnabled(true);
      for(unsigned int i=0;i<combines.size();i++)
	if((*combo)==(*((ATrackCombination*)combines[i])))
	  {
	    combTrack->setVisible(false);
	    combName=combines[i]->name();
	    break;
	  }
      
      header->setPlainText("MULTIPLE TRACKS SELECTED");

      name->setHtml("<b>Name:</b> "+combName);
      charge->setHtml("<b>Charge:</b> "+QString::number(combo->charge()));
      invmass->setHtml("<b>Invariant Mass:</b> "+QString::number(combo->getInvariantMass()));
      
      header->setVisible(true);
      name->setVisible(true);
      charge->setVisible(true);
      invmass->setVisible(true);
      pt->setVisible(false);
      id->setVisible(false);
      icon->setVisible(false);
    }
  
  // Show the add button if at least one of the tracks are not in the list already...
  QList<ATrack*> tracks=analysisData->getCollection("bookmarked_tracks");
  addTrack->setVisible(false);
  for (int i=0;i<combo->size();i++)
    {
      if ( !tracks.contains((*combo)[i]) )
        {
	  addTrack->setVisible (true );
        }
    }

  emit combineButtonEnabled(combTrack->isVisible());

}
