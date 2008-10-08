#include "ASelectionInfoScene.h"

#include <QInputDialog>

ASelectionInfoScene::ASelectionInfoScene(QObject* parent):QGraphicsScene(parent)
{
    combo=new ATrackCombination;
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

  id=addText("ID");
  id->setPos(0,55);
  id->setDefaultTextColor("white");

  nonSelectable=addText("Non Selectable");
  nonSelectable->setPos(230,25);
  nonSelectable->setDefaultTextColor("black");

  pt=addText("Pt:");
  pt->setPos(130,25);
  pt->setDefaultTextColor("white");
  eta=addText("Eta:");
  eta->setPos(130,40);
  eta->setDefaultTextColor("white");
  phi=addText("Phi:");
  phi->setPos(130,55);
  phi->setDefaultTextColor("white");

  // Buttons
  QPushButton *_addTrack=new QPushButton("Add to List");
  addTrack=addWidget(_addTrack);
  addTrack->setVisible(true);
  addTrack->setPos(220,20);
  connect(_addTrack,SIGNAL(clicked()),  this,SLOT(handleAddTrack()));

  QPushButton *_combTrack=new QPushButton("Combine Tracks");
  combTrack=addWidget(_combTrack);
  combTrack->setVisible(true);
  combTrack->setPos(220,50);
  connect(_combTrack,SIGNAL(clicked()),  this,SLOT(handleCombTracks()));

  // Main message
  serifFont.setPointSize(14);
  message=addText("",serifFont);
  message->setPos(20,10);
  message->setVisible(false);


  // Hide Everything by Default
  QList<QGraphicsItem*> _items=items();
  for (int i=0;i<_items.size();i++)
    {
      _items[i]->setVisible(false);
    }
}

void ASelectionInfoScene::handleAddTrack()
{
    addTrack->setVisible(false);
    emit addButtonEnabled(false);
    for (int i=0;i<combo->size();i++)
        emit sig_addTrackToTable((*combo)[i]);
}

void ASelectionInfoScene::handleCombTracks()
{
    bool ok;

    QString name=QInputDialog::getText(0,tr("Track Combination Name"),tr("Combination Name:"),QLineEdit::Normal,combo->getName(),&ok);

    if (ok) //Proceed only if OK clicked
    {
        combo->setName(name);
        emit sig_addComboToTable(combo);
        combTrack->setVisible(false);
	emit combineButtonEnabled(false);

        combo=new ATrackCombination(); //Make sure we release ownership of the current combination. This little baby is out in the world on it's own from now on...
    }
}

void ASelectionInfoScene::handleViewportChange(int from,int to)
{
    switch (to)
    {
    case AGeometry::Cam3D:
      hideMessage();
      break;
    case AGeometry::Orthogonal:
    case AGeometry::Projective:
      displayMessage("Track selection only supported<br/> in first-person camera mode.");
      break;
    }
}

void ASelectionInfoScene::displayMessage(QString text)
{
    //TODO: Automatic line breaks and centering using font metrics
    message->setHtml("<center>"+text+"</center>");
    message->setVisible(true);

    //Hide track information
    header->setVisible(false);
    nonSelectable->setVisible(false);
    name->setVisible(false);
    charge->setVisible(false);
    pt->setVisible(false);
    eta->setVisible(false);
    phi->setVisible(false);
    id->setVisible(false);
    addTrack->setVisible(false);
    emit addButtonEnabled(false);
    combTrack->setVisible(false);
    emit combineButtonEnabled(false);
}

void ASelectionInfoScene::hideMessage()
{
    message->setVisible(false);
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
        nonSelectable->setVisible(false);
        name->setVisible(false);
        charge->setVisible(false);
        pt->setVisible(false);
        eta->setVisible(false);
        phi->setVisible(false);
        id->setVisible(false);
        addTrack->setVisible(false);
        combTrack->setVisible(false);
        message->setVisible(false);
        return; //Dooone;
    }
    else if (combo->size()==1)
        //Only 1 thing, we can handle that
    {
        if ( (*combo)[0]->Type == 1 ) //track
        {
            header->setPlainText("SELECTED TRACK INFO");
            name->setHtml("<b>Name:</b> "+QString((*combo)[0]->name.c_str()));
            charge->setHtml("<b>Charge:</b> "+QString::number((*combo)[0]->node->getCharge()));
            pt->setHtml("<b>Pt:</b> "+QString::number((*combo)[0]->node->getPt()));
            eta->setHtml("<b>Eta:</b> "+QString::number((*combo)[0]->node->getEta()));
            phi->setHtml("<b>Phi:</b> "+QString::number((*combo)[0]->node->getPhi()));
            id->setHtml("<b>id:</b> "+QString::number((*combo)[0]->trackID));
            nonSelectable->setHtml("<b>This track is<br>irrelevant for<br>the analysis</b>");

            header->setVisible(true);
            name->setVisible(true);
            charge->setVisible(true);
            pt->setVisible(true);
            eta->setVisible(true);
            phi->setVisible(true);
            id->setVisible(true);
            combTrack->setVisible(false);
	    emit combineButtonEnabled(false);
            nonSelectable->setVisible(false);
        }
        else if ( (*combo)[0]->Type == 2 ) //jet
        {
            header->setPlainText("SELECTED JET INFO");
            name->setHtml("<b>Type:</b> Jet");
            eta->setHtml("<b>Eta:</b> "+QString::number((*combo)[0]->node->getEta()));
            phi->setHtml("<b>Phi:</b> "+QString::number((*combo)[0]->node->getPhi()));
            pt->setHtml("<b>Pt:</b> "+QString::number((*combo)[0]->node->getEt()));

            header->setVisible(true);
            name->setVisible(true);
            charge->setVisible(false);
            pt->setVisible(true);
            eta->setVisible(true);
            phi->setVisible(true);
            combTrack->setVisible(false);
        }
        else if ( (*combo)[0]->Type == 4 ) //Missing Et
        {
            header->setPlainText("SELECTED MisEt INFO");
            name->setHtml("<b>Type:</b> Missing Et");
            eta->setHtml("<b>etx:</b> "+QString::number((*combo)[0]->node->getEtx()));
            phi->setHtml("<b>ety:</b> "+QString::number((*combo)[0]->node->getEty()));
            pt->setHtml("<b>Et:</b> "+QString::number((*combo)[0]->node->getEt()));

            header->setVisible(true);
            name->setVisible(true);
            charge->setVisible(false);
            pt->setVisible(true);
            eta->setVisible(true);
            phi->setVisible(true);
            combTrack->setVisible(false);
        }
    }
    else
        //Multi-track selection
    {
        header->setPlainText("MULTIPLE TRACKS SELECTED");
        //TODO: Print out some cool information, I'm guessing
        name->setHtml("<b>Name:</b> Unknown Combination");
        charge->setHtml("<b>Charge:</b> "+QString::number(combo->getNetCharge()));
        nonSelectable->setHtml("<b>At least one of the<br>selected tracks<br>is irrelevant for<br>the analysis</b>");

        header->setVisible(true);
        name->setVisible(true);
        charge->setVisible(true);
        pt->setVisible(false);
        eta->setVisible(false);
        phi->setVisible(false);
        id->setVisible(false);
        nonSelectable->setVisible(false);


        combTrack->setVisible(true);
    }

    // Show the add button if at least one of the tracks are not in the list already...
    addTrack->setVisible(false);
    for (int i=0;i<combo->size();i++)
    {
        if (!particleFilter((*combo)[i]))
        {
            nonSelectable->setVisible(true);
        }

        if ( (!(*combo)[i]->isInList))
        {
            addTrack->setVisible (true );
        }

    }

    if ((nonSelectable->isVisible()))
    {
        combTrack->setVisible(false);
        addTrack->setVisible (false );
    }

    qDebug() << "Emit! " << combTrack->isVisible();
    emit addButtonEnabled(addTrack->isVisible());
    emit combineButtonEnabled(combTrack->isVisible());
}

bool ASelectionInfoScene::particleFilter(ATrack* strack) // We don't want to allow every track to be added to the table
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
