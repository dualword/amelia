#include "ATrack.h"




void ASTrack::createTrackStructure( scene::ISceneNode* parent, ISceneManager* smgr,  s32 ID )
{
  ASTrack3DNode* HelixNode = new ASTrack3DNode ( parent, smgr, ID ,this);
  HelixNode->vividColor = trackColor;



    if ( ( name == "mu-" || name == "mu+" || name == "e+" || name == "e-" ) && pt >= 2 )
    {
        style = 1;
    }
    else
    {
        style = 1;
    }
    HelixNode->setTrackStyle(style);

    tL = HelixNode->tL;
    Mlv = 0;

    node = HelixNode;
    selectionID = 0;
    isInList = false;
}

void ASTrack::select()
{
    if (node)
        node->setTrackStyle ( 3 );
}

void ASTrack::deselect()
{
    if (node)
        node->setTrackStyle ( style );
}



void AJet::createTrackStructure(scene::ISceneNode* parent, ISceneManager* smgr,  s32 ID)
{
    AJet3DNode* JetNode = new AJet3DNode ( parent, smgr, ID );
    style = 7;

    JetNode->setTrack ( this );
    JetNode->createJetPyramids();

    node = JetNode;

    selectionID = 0;
    isInList = false;

}

void AJet::select()
{
    if (node)
        node->setTrackStyle ( 6 );
}

void AJet::deselect()
{
    if (node)
        node->setTrackStyle ( style );
}


void AMisET::createTrackStructure(scene::ISceneNode* parent, ISceneManager *smgr,  s32 ID)
{
    AMisET3DNode* MisET3DNode = new AMisET3DNode (parent, smgr, ID );
    style = 11;

    MisET3DNode->setTrack ( this );
    MisET3DNode->createMisEtBoxes();
    node = MisET3DNode;

    selectionID = 0;
    isInList = false;
}

void AMisET::select()
{
    if (node)
        node->setTrackStyle ( 10 );
}

void AMisET::deselect()
{
    if (node)
        node->setTrackStyle ( style );
}



