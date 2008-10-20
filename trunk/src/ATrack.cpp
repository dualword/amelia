#include "ATrack.h"




void ASTrack::createTrackStructure( scene::ISceneNode* parent, AGeometry* base,  s32 ID )
{
    ASTrack3DNode* HelixNode = new ASTrack3DNode ( parent, base, ID );
    node = HelixNode;
    node->charge = q;
    node->eta = eta;
    node->phi = phi;
    pt = et;
    node->pt = pt;
    node->v_phi = phiVertex;
    node->v_rho = rhoVertex;
    node->v_z = zVertex;
    node->vividColor = trackColor;
    node->calculateDimmedColors();
    node->setTrack ( this );
    node->type = 1; //0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
    style = 1;
    node->style = 1;


    if ( q == 0 )
    {
        std::vector<core::vector3df> StartEndNeutral = HelixNode->getNeutralPath();
        start = StartEndNeutral.front();
        end = StartEndNeutral.back();
        node->start = start;
        node->end = end;
        node->curvePoints.push_back ( core::vector3df ( 0,0,0 ) );
        node->createBoxesNeutral();

    }
    else
    {
        maxAngle = HelixNode->getChargedMaxAngle();
        node->maxAngle = maxAngle;
        node->createCurveVector();
        node->createBoxesCharged();

    }

    if ( ( name == "mu-" || name == "mu+" || name == "e+" || name == "e-" ) && pt >= 2 )
    {
        style = 1;
    }
    else
    {
        style = 1;
    }

    node->boxMode = false;
    node->style = style;
    node->setTrackStyle ( style );
    tL = node->tL;
    Mlv = node->Mlv;

    selectionID = 0;
    isInList = false;
}


void AJet::createTrackStructure(scene::ISceneNode* parent, AGeometry* base,  s32 ID)
{
    AJet3DNode* JetNode = new AJet3DNode ( parent, base, ID );
    node = JetNode;
    node->eta = eta;
    node->phi = phi;
    style = 7;

    node->type = 2; //0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
    node->setTrack ( this );
    node->createJetPyramids();


    selectionID = 0;
    isInList = false;

}


void AMisET::createTrackStructure(scene::ISceneNode* parent, AGeometry* base,  s32 ID)
{
    AMisET3DNode* MisET3DNode = new AMisET3DNode (parent, base, ID );
    node = MisET3DNode;
    node->etx = etx;
    node->ety = ety;
    style = 11;

    node->type = 4; //0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
    node->setTrack ( this );
    node->createMisEtBoxes();
    selectionID = 0;
    isInList = false;
}



