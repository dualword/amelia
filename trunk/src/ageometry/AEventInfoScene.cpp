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
    header=addText("SELECTED EVENT INFO",serifFont);
    header->setPos(0,0);
    header->setDefaultTextColor("red");

    labelDisplayed=addText("");
    labelDisplayed->setPos(160,20);
    labelDisplayed->setDefaultTextColor("black");

    labelTotal=addText("");
    labelTotal->setPos(120,20);
    labelTotal->setDefaultTextColor("black");

    dataLabels=addText("");
    dataLabels->setPos(0,40);
    dataLabels->setDefaultTextColor("blue");

    dataComplete=addText("");
    dataComplete->setPos(120,40);
    dataComplete->setDefaultTextColor("blue");

    dataDisplayed=addText("");
    dataDisplayed->setPos(170,40);
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
      header->setHtml("<html>\n <b>SELECTED EVENT INFO</b></html>");
      labelTotal->setHtml("<b>Total</b>");
      labelDisplayed->setHtml("<b>Visible</b>");
      
      dataLabels->setHtml("<html>\n <b>Tracks:<br/>Neutral Hadrons: <br/>Charged Hadrons:<br/>Photons: <br/>Muons:<br/>Electrons:</b></html>");
      
      dataComplete->setHtml("<html>\n"
			    +QString::number(complete->numTracks)+"<br/>"
			    +QString::number(complete->numNeutralHadrons)+"<br/>"
			    +QString::number(complete->numChargedHadrons)+"<br/>"
			    +QString::number(complete->numPhotons)+"<br/>"
			    +QString::number(complete->numMuons)+"<br/>"
			    +QString::number(complete->numElectrons)+"<br/>"
			    +"</html>");
      
      dataDisplayed->setHtml("<html>\n"
			     +QString::number(_event->numTracks)+"<br/>"
			     +QString::number(_event->numNeutralHadrons)+"<br/>"
			     +QString::number(_event->numChargedHadrons)+"<br/>"
			     +QString::number(_event->numPhotons)+"<br/>"
			     +QString::number(_event->numMuons)+"<br/>"
			     +QString::number(_event->numElectrons)+"<br/>"
			     +"</html>");
    }
  else
    {
      header->setHtml("<html>\n <b>NO EVENT LOADED</b></html>");
    }
}
