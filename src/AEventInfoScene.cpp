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

XmlEvent* AEventInfoScene::event()
{
    return _event;
}

void AEventInfoScene::setEvent(XmlEvent* e)
{
    _event=e;
    connect(e,SIGNAL(eventChanged()),
            this,SLOT(updateEventInfo()));
    updateEventInfo();
}

void AEventInfoScene::updateEventInfo()
{

    if ( _event->EventComplete.Tracks.size() > 0 )
    {
        header->setHtml("<html>\n <b>SELECTED EVENT INFO</b></html>");
        labelTotal->setHtml("<b>Total</b>");
        labelDisplayed->setHtml("<b>Visible</b>");

        dataLabels->setHtml("<html>\n <b>Tracks:<br/>Neutral Hadrons: <br/>Charged Hadrons:<br/>Photons: <br/>Muons:<br/>Electrons:</b></html>");

        dataComplete->setHtml("<html>\n"
                              +QString::number(_event->EventComplete.numTracks)+"<br/>"
                              +QString::number(_event->EventComplete.numNeutralHadrons)+"<br/>"
                              +QString::number(_event->EventComplete.numChargedHadrons)+"<br/>"
                              +QString::number(_event->EventComplete.numPhotons)+"<br/>"
                              +QString::number(_event->EventComplete.numMuons)+"<br/>"
                              +QString::number(_event->EventComplete.numElectrons)+"<br/>"
                              +"</html>");

        dataDisplayed->setHtml("<html>\n"
                               +QString::number(_event->Event.numTracks)+"<br/>"
                               +QString::number(_event->Event.numNeutralHadrons)+"<br/>"
                               +QString::number(_event->Event.numChargedHadrons)+"<br/>"
                               +QString::number(_event->Event.numPhotons)+"<br/>"
                               +QString::number(_event->Event.numMuons)+"<br/>"
                               +QString::number(_event->Event.numElectrons)+"<br/>"
                               +"</html>");
    }
    else
    {
       header->setHtml("<html>\n <b>NO EVENT LOADED</b></html>");
    }
}

    bool AEventInfoScene::particleFilter(ATrack* strack) // We don't want to allow every track to be added to the table
    {
        if ((strack->Type == 2) || (strack->Type == 4) || (strack->name == "e-")  || (strack->name == "e+") || (strack->name == "mu-") || (strack->name == "mu+") || (strack->name == "gamma") || (strack->name == "e-"))
        {
            return true;
        }
        else
        {
            return false;
        }

    }
