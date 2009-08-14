#include "AEventInfoScene.h"

#include <QInputDialog>

AEventInfoScene::AEventInfoScene(QObject* parent):QGraphicsScene(parent),_event(0)
{
    init();
}

AEventInfoScene::~AEventInfoScene()
{}

void AEventInfoScene::init()
{
    QFont serifFont("Times", 10, QFont::Bold);
    header=addText("NO EVENT LOADED",serifFont);
    header->setPos(0,0);
    header->setDefaultTextColor("red");

    runInfo=addText("");
    runInfo->setPos(0,20);
    runInfo->setDefaultTextColor("black");

    eventInfo=addText("");
    eventInfo->setPos(100,20);
    eventInfo->setDefaultTextColor("black");

    metInfo=addText("");
    metInfo->setPos(0,50);
    metInfo->setDefaultTextColor("black");

    labelDisplayed=addText("");
    labelDisplayed->setPos(160,70);
    labelDisplayed->setDefaultTextColor("black");

    labelTotal=addText("");
    labelTotal->setPos(120,70);
    labelTotal->setDefaultTextColor("black");

    dataLabels=addText("");
    dataLabels->setPos(0,85);
    dataLabels->setDefaultTextColor("blue");

    dataComplete=addText("");
    dataComplete->setPos(120,85);
    dataComplete->setDefaultTextColor("blue");

    dataDisplayed=addText("");
    dataDisplayed->setPos(170,85);
    dataDisplayed->setDefaultTextColor("blue");
}

void AEventInfoScene::setEvent(AFilteredEvent* e)
{
  if(!_event)
    {
      disconnect(e,SIGNAL(filtersUpdated()),
		 this,SLOT(updateEventInfo()));
    }

  connect(e,SIGNAL(filtersUpdated()),
	  this,SLOT(updateEventInfo()));

  _event=e;

  updateEventInfo();
}

void AEventInfoScene::updateEventInfo()
{
  if ( _event )
    {
      AEvent* complete=_event->completeEvent();
      header->setHtml("<html>\n <b>CURRENT EVENT INFO</b></html>");
      if(_event->MisET.size()>0)
	metInfo->setHtml("<b>Missing Et:</b> "+QString::number(_event->MisET[0]->et())+" GeV");
      else
	metInfo->setHtml("<b>Missing Et:</b> unknown");

      runInfo->setHtml("<b>Run:</b> "+QString::number(_event->runNumber));
      eventInfo->setHtml("<b>Event:</b> "+QString::number(_event->eventNumber));
      
      labelTotal->setHtml("<b>Total</b>");
      labelDisplayed->setHtml("<b>Visible</b>");
      
      dataLabels->setHtml("<html>\n <b>All Particles:<br/>Hadrons:<br/>Photons: <br/>Muons:<br/>Electrons:<br/>Jets:<br/></b></html>");
      
      dataComplete->setHtml("<html>\n"
			    +QString::number(complete->numParticles)+"<br/>"
			    +QString::number(complete->numNeutralHadrons+complete->numChargedHadrons)+"<br/>"
			    +QString::number(complete->numPhotons)+"<br/>"
			    +QString::number(complete->numMuons)+"<br/>"
			    +QString::number(complete->numElectrons)+"<br/>"
			    +QString::number(complete->numJets)+"<br/>"
			    +"</html>");
      
      dataDisplayed->setHtml("<html>\n"
			     +QString::number(_event->numParticles)+"<br/>"
			     +QString::number(_event->numNeutralHadrons+_event->numChargedHadrons)+"<br/>"
			     +QString::number(_event->numPhotons)+"<br/>"
			     +QString::number(_event->numMuons)+"<br/>"
			     +QString::number(_event->numElectrons)+"<br/>"
			     +QString::number(_event->numJets)+"<br/>"
			     +"</html>");
    }
  else
    {
      header->setHtml("<html>\n <b>NO EVENT LOADED</b></html>");
    }
}
