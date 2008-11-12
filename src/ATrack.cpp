#include "ATrack.h"




void ASTrack::createTrackStructure( scene::ISceneNode* parent, AGeometry* base,  s32 ID )
{
    ASTrack3DNode* HelixNode = new ASTrack3DNode ( parent, base, ID );
    HelixNode->vividColor = trackColor;
    HelixNode->calculateDimmedColors();
    HelixNode->setTrack ( this );
    HelixNode->type = 1; //0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
    style = 1;
    HelixNode->style = 1;


    if ( q == 0 )
    {
        std::vector<core::vector3df> StartEndNeutral = HelixNode->getNeutralPath();
        start = StartEndNeutral.front();
        end = StartEndNeutral.back();
        HelixNode->start = start;
        HelixNode->end = end;
        HelixNode->curvePoints.push_back ( core::vector3df ( 0,0,0 ) );
        HelixNode->createBoxesNeutral();

    }
    else
    {
        maxAngle = HelixNode->getChargedMaxAngle();
        HelixNode->maxAngle = maxAngle;
        HelixNode->createCurveVector();
        HelixNode->createBoxesCharged();

    }

    if ( ( name == "mu-" || name == "mu+" || name == "e+" || name == "e-" ) && pt >= 2 )
    {
        style = 1;
    }
    else
    {
        style = 1;
    }

    HelixNode->boxMode = false;
    HelixNode->style = style;
    HelixNode->setTrackStyle ( style );
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



void AJet::createTrackStructure(scene::ISceneNode* parent, AGeometry* base,  s32 ID)
{
    AJet3DNode* JetNode = new AJet3DNode ( parent, base, ID );
    style = 7;

    JetNode->type = 2; //0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
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


void AMisET::createTrackStructure(scene::ISceneNode* parent, AGeometry* base,  s32 ID)
{
    AMisET3DNode* MisET3DNode = new AMisET3DNode (parent, base, ID );
    style = 11;

    MisET3DNode->type = 4; //0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
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



