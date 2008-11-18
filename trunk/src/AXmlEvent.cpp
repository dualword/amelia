/**************** License agreement ******************

AMELIA Copyright (c) 2008, The Regents of the University of California,
through Lawrence Berkeley National Laboratory.  All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

(1) Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

(3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
U.S. Dept. of Energy nor the names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

You are under no obligation whatsoever to provide any bug fixes, patches, or upgrades to the features,
functionality or performance of the source code ("Enhancements") to anyone; however, if you choose to
make your Enhancements available either publicly, or directly to Lawrence Berkeley National Laboratory,
without imposing a separate written license agreement for such Enhancements, then you hereby grant the
following license: a  non-exclusive, royalty-free perpetual license to install, use, modify,
prepare derivative works, incorporate into other computer software, distribute,
and sublicense such enhancements or derivative works thereof, in binary and source code form.
******************************************************/


#include "AXmlEvent.h"

#include <QFile>

int AXmlEvent::ptcut=1000;

QMap<QString,AXmlEvent*> AXmlEvent::cache=QMap<QString,AXmlEvent*>();


AXmlEvent* AXmlEvent::CachedEvent(QString file)
{
    if (cache.contains(file))
        return cache[file];

    AXmlEvent* event=new AXmlEvent();
    event->LoadEvent(file.toLocal8Bit().data());
    cache[file]=event;
    return event;
}


AXmlEvent::AXmlEvent()
{
    //Initialize the bool vector describing which type of particle is visible
    for (int i=0; i < 8 ; i++)
    {
        P_checkbox_states.push_back(true);
    }

    //Define the default reconstruction model for Missing ET and Jets
    currentJetType = AJet::jKt4H1TowerJets;
    currentMisEtType = AMisET::mMET_Final;
    highestTrackID = 0;

}

AXmlEvent::~AXmlEvent()
{
}



std::vector<int> AXmlEvent::getDataInt ( QDomNode xml )
{
    std::string sb;
    std::string add;
    vector<int>v;
    int sbi;

    QDomText textNode=xml.firstChild().toText();
    std::string dstr = textNode.data().trimmed().toLocal8Bit().data();

    for ( unsigned int i=0; i < dstr.size(); i++ )
    {
        if ( ( dstr[i] != ' ' ) && ( dstr[i] != '\n' ) )
        {
            add = dstr[i];
            sb = sb + add;
        }

        else
        {
            sbi = atoi ( sb.c_str() );
            if ( sbi != 0 )
            {
                v.push_back ( sbi );
                sb = "";
                //j++;
            }
        }
    }

    return v;
}

std::vector<float> AXmlEvent::getDataFloat ( QDomNode xml )
{
    std::string sb;
    std::string add;
    vector<float>v;
    float sbf;

    QDomText textNode=xml.firstChild().toText();
    std::string dstr = textNode.data().toLocal8Bit().data();

    for ( unsigned int i=0; i < dstr.size(); i++ )
    {
        if ( ( dstr[i] != ' ' ) && ( dstr[i] != '\n' ) )
        {
            add = dstr[i];
            sb = sb + add;
        }

        else
        {
            sbf = atof ( sb.c_str() );
            if ( sbf != 0 )
            {
                v.push_back ( sbf );
                sb = "";
            }
        }
    }

    return v;
}

std::vector <AJet*> AXmlEvent::GetJetsFromDOM ( QDomDocument dom , AEvent* event)
{
    // Get all the jet data from the XML file. There are four main jet types of interest, expressed as attributes in the file
    // Types are Kt4H1TopoJets, Cone4H1TopoJets, Kt4H1TowerJets and Cone4H1TowerJets

    std::vector <AJet*> jets;

    //elements of the Jets
    vector<float> eta;
    vector<float> phi;
    vector<float> et;
    vector<float> numCells;


    QDomNodeList JetNodes=dom.elementsByTagName("Jet");

    //Load the Jets
    for (unsigned int i=0;i<JetNodes.length();i++)
    {
        QDomElement node=JetNodes.at(i).toElement();

        // storeGateKey is the attribute that distinguishes different reconstruction models apart
        QDomAttr attr = node.attributeNode( "storeGateKey" );

        et = getDataFloat ( node.elementsByTagName("et").at(0) );
        eta = getDataFloat ( node.elementsByTagName("eta").at(0) );
        phi = getDataFloat ( node.elementsByTagName("phi").at(0) );
        numCells = getDataFloat ( node.elementsByTagName("numCells").at(0) );


        // Now we should load every node individually, and assign the proper type to them
        for ( unsigned int s = 0; s < et.size(); s++ )
        {
            AJet* j = new AJet();
            j->et = et[s];
            j->pt = j->et;
            if ( s<eta.size() ) j->eta = eta[s];
            if ( s<phi.size() ) j->phi = phi[s];
            if ( s<numCells.size() ) j->numCells = numCells[s];

            if (attr.value() == QString("Kt4H1TopoJets"))
            {
                j->jtype = AJet::jKt4H1TopoJets;
                qDebug() << "New Kt4H1TopoJet added ";
            }
            if (attr.value() == QString("Cone4H1TopoJets"))
            {
                j->jtype = AJet::jCone4H1TopoJets;
                qDebug() << "New Cone4H1TopoJet added ";
            }
            if (attr.value() == QString("Kt4H1TowerJets"))
            {
                j->jtype = AJet::jKt4H1TowerJets;
                qDebug() << "New Kt4H1TowerJet added ";
            }
            if (attr.value() == QString("Cone4H1TowerJets"))
            {
                j->jtype = AJet::jCone4H1TowerJets;
                qDebug() << "New Cone4H1TowerJet added ";
            }
            j->Type = j->eJet;
            j->name = "Jet";
            j->trackID = ++highestTrackID;
            jets.push_back ( j );
            event->Tracks.push_back(j);


        }

        qDebug() << "Loaded "<< eta.size() << " new jets of type " << attr.value();
        qDebug() << "Total # Jets so far: " << jets.size();



    }

    qDebug() << "Number of Jet Nodes: " << JetNodes.length();

    return jets;

}

std::vector <AMisET*> AXmlEvent::GetMisETFromDOM ( QDomDocument dom , AEvent* event)
{
    // Get all the Missing ET data from the XML file. There are five main types of MisET, expressed as attributes in the file
    // Types are MET_Final, MET_RefMuon, MET_Calib, MET_RefFinal and MET_Truth

    std::vector <AMisET*> MET;

    //elements of MisET
    vector<float> etx;
    vector<float> ety;
    vector<float> et;


    QDomNodeList METNodes=dom.elementsByTagName("ETMis");

    //Load the Jets
    for (unsigned int i=0;i<METNodes.length();i++)
    {
        QDomElement node=METNodes.at(i).toElement();

        // storeGateKey is the attribute that distinguishes different reconstruction models apart
        QDomAttr attr = node.attributeNode( "storeGateKey" );

        et = getDataFloat ( node.elementsByTagName("et").at(0) );
        etx = getDataFloat ( node.elementsByTagName("etx").at(0) );
        ety = getDataFloat ( node.elementsByTagName("ety").at(0) );

        // Now we should load every node individually, and assign the proper type to them
        for ( unsigned int s = 0; s < et.size(); s++ )
        {
            AMisET* m = new AMisET();
            m->et = et[s];
            m->pt = m->et;
            if ( s<etx.size() ) m->etx = etx[s];
            if ( s<ety.size() ) m->ety = ety[s];

            if (attr.value() == QString("MET_Final"))
            {
                m->mtype = AMisET::mMET_Final;
                qDebug() << "New MET_Finalt added ";
            }
            if (attr.value() == QString("MET_RefMuon"))
            {
                m->mtype = AMisET::mMET_RefMuon;
                qDebug() << "New MET_RefMuon added ";
            }
            if (attr.value() == QString("MET_Calib"))
            {
                m->mtype = AMisET::mMET_Calib;
                qDebug() << "New MET_Calib added ";
            }
            if (attr.value() == QString("MET_RefFinal"))
            {
                m->mtype = AMisET::mMET_RefFinal;
                qDebug() << "New MET_RefFinal added ";
            }
            if (attr.value() == QString("MET_Truth"))
            {
                m->mtype = AMisET::mMET_Truth;
                qDebug() << "New MET_Truth added ";
            }
            m->Type = m->eMissingEt;
            m->trackID = ++highestTrackID;
            m->name = "MissingET";
            MET.push_back ( m );
            event->Tracks.push_back(m);

        }


    }

    qDebug() << "Number of MisET Nodes: " << METNodes.length();

    return MET;

}

std::vector <ASTrack*> AXmlEvent::GetSTracksFromDOM ( QDomDocument dom , AEvent* event )
{
    // Aux variables and definitions
    std::vector <ASTrack*> tracks;



    //elements of the simulated tracks
    vector<int> str_code;
    vector<float> str_eta;
    vector<int> str_id;
    vector<float> str_phi;
    vector<float> str_phiVertex;
    vector<float> str_pt;
    vector<float> str_rhoVertex;
    vector<float> str_zVertex;
    vector<float> str_q;
    vector<float> v;

    const int codelist[52] =
    {
        0, 11, 12, 13, 14, 15, 16, 22, 23, 24, 111, 113, 130,
        211, 221, 310, 321, 411, 421, 431, 2112, 2212, 3112, 3122, 3212, 3222,
        3312, 3322, 3334, 4122, -11, -12, -13, -14, -15, -16, -24, -211, -321,
        -411, -421, -431, -2112, -2212, -3112, -3122, -3212, -3222, -3312,
        -3322, -3334, -4122
    };

    const int chargelist[52] =
    {
        0, -1, 0, -1, 0, -1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1,
        0, 1, 0, 1, -1, 0, 0, 1, -1, 0, -1, 1, 1, 0, 1, 0, 1, 0, -1, -1, -1, -1, 0,
        -1, 0, -1, 1, 0, 0, -1, 1, 0, 1, -1
    };

    const std::string namelist[52] =
        { "0", "e-", "nu_e", "mu-", "nu_mu", "tau-", "nu_tau",
          "gamma", "Z0", "W+", "pi0", "rho0", "K_L0", "pi+", "eta", "K_S0", "K+",
          "D+", "D0", "D_s+", "n0", "p+", "Sigma-", "Lambda0", "Sigma0", "Sigma+",
          "Xi-", "Xi0", "Omega-", "Lambda_c+", "e+", "nu_ebar", "mu+", "nu_mubar",
          "tau+", "nu_taubar", "W-", "pi-", "K-", "D-", "Dbar0", "D_s-", "nbar0",
          "pbar-", "Sigmabar+", "Lambdabar0", "Sigmabar0", "Sigmabar-", "Xibar+",
          "Xibar0", "Omegabar+", "Lambda_cbarminus"
        };

    const video::SColor colorlist[52] =
    {
        video::SColor ( 0,0,0,0 ), video::SColor ( 0,255,255,0 ), video::SColor ( 0,55,55,55 ), video::SColor ( 0,0,0,255 ), video::SColor ( 0,55,55,55 ),
        video::SColor ( 0,255,255,0 ), video::SColor ( 0,55,55,55 ), video::SColor ( 0,0,255,0 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ),
        video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,111,111,111 ), video::SColor ( 0,77,77,77 ), video::SColor ( 0,255,255,255 ),
        video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,0 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ),
        video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ),video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,0 ), video::SColor ( 0,55,55,55 ), video::SColor ( 0,0,0,255 ), video::SColor ( 0,55,55,55 ),
        video::SColor ( 0,255,255,0 ), video::SColor ( 0,55,55,55 ), video::SColor ( 0,255,0,0 ),video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ),
        video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 ),
        video::SColor ( 0,255,255,255 ), video::SColor ( 0,255,0,0 ), video::SColor ( 0,255,0,0 )
    };


    //Get the list of different types of "tracks"
    QDomNodeList ETMis=dom.elementsByTagName("ETMis");
    QDomNodeList STr=dom.elementsByTagName("STr");

    //Load the cool tracks!
    for (unsigned int i=0;i<STr.length();i++)
    {
        QDomElement node=STr.at(i).toElement();
        str_code = getDataInt ( node.elementsByTagName("code").at(0) );
        str_eta = getDataFloat ( node.elementsByTagName("eta").at(0) );
        str_id = getDataInt ( node.elementsByTagName("id").at(0) );
        str_phi = getDataFloat ( node.elementsByTagName("phi").at(0) );
        str_phiVertex = getDataFloat ( node.elementsByTagName("phiVertex").at(0) );
        str_pt = getDataFloat ( node.elementsByTagName("pt").at(0) );
        str_rhoVertex = getDataFloat ( node.elementsByTagName("rhoVertex").at(0) );
        str_zVertex = getDataFloat ( node.elementsByTagName("zVertex").at(0) );
    }


    // Tracks section
    for ( unsigned int i = 0; i < str_code.size(); i++ )
    {

        ASTrack* t = new ASTrack();

        t->name = "unknown";
        for ( unsigned int j = 0; j < 52; j++ )
        {
            if ( codelist[j] == str_code[i] )
            {
                t->q = chargelist[j];
                t->name = namelist[j];
                t->trackColor = colorlist[j];
                //cout << "\n STrack of type " << t->name ;
            }
        }


        if ( i<str_code.size() ) t->code = str_code[i];
        if ( i<str_eta.size() ) t->eta = str_eta[i];
        if ( i<str_id.size() ) t->trackID = str_id[i];
        if ( i<str_phi.size() ) t->phi = str_phi[i];
        if ( i<str_phiVertex.size() ) t->phiVertex = str_phiVertex[i];
        if ( i<str_pt.size() ) t->pt = str_pt[i];
        if ( i<str_rhoVertex.size() ) t->rhoVertex = str_rhoVertex[i];
        if ( i<str_zVertex.size() ) t->zVertex = str_zVertex[i];
        t->Type = t->eSTrack;
        if (t->trackID > highestTrackID)
            highestTrackID = t->trackID;

        //cout << "\n track of type " << t->Type ;
        //cout << "\n name: " << t->name ;
        tracks.push_back ( t );
        event->Tracks.push_back(t);

        // cout << "\nSanity test. STrack code: " << t->code << " Which should be the same as " << static_cast<ASTrack*>((event->Tracks.at(event->Tracks.size()-1)))->code;
    }

    return tracks;
}

std::vector <shower> AXmlEvent::GetShowersFromDOM ( QDomDocument dom, char* calo )
{
    //gets data for all HEC, LAr, and TILE showers in event; arguments are xml file name, device, and calorimeter type

    std::vector <shower> showers;
    shower s;

    vector<float> energy;
    vector<float> eta;
    vector<int> id;
    vector<int> layer;
    vector<float> phi;
    vector<int> sub;

    QDomNodeList Calo=dom.elementsByTagName(calo);

    //Load the calometers
    for (unsigned int i=0;i<Calo.length();i++)
    {
        QDomElement node=Calo.at(i).toElement();
        energy = getDataFloat ( node.elementsByTagName("energy").at(0) );
        eta = getDataFloat ( node.elementsByTagName("eta").at(0) );
        id = getDataInt ( node.elementsByTagName("id").at(0) );
        layer = getDataInt ( node.elementsByTagName("layer").at(0) );
        phi = getDataFloat ( node.elementsByTagName("phi").at(0) );
        sub = getDataInt ( node.elementsByTagName("sub").at(0) );
    }

    for (unsigned int i = 0; i < energy.size(); i++ )
    {
        s.energy = energy[i];
        if ( i<eta.size() ) s.eta = eta[i];
        if ( i<id.size() ) s.trackID = id[i];
        if ( i<layer.size() ) s.layer = layer[i];
        if ( i<phi.size() ) s.phi = phi[i];
        if ( i<sub.size() ) s.sub = sub[i];
        showers.push_back ( s );
    }

    return showers;
}

std::vector <FCALshower> AXmlEvent::GetFCALShowersFromDOM ( QDomDocument dom )
{
    //gets data for all HEC showers in event

    std::vector <FCALshower> showers;
    FCALshower s;

    int i=0;

    vector<float> dx;
    vector<float> dy;
    vector<float> energy;
    vector<int> id;
    vector<int> layer;
    vector<int> sub;
    vector<float> x;
    vector<float> y;

    QDomNodeList FCAL=dom.elementsByTagName("FCAL");

    for (unsigned int i=0;i<FCAL.length();i++)
    {
        QDomElement node=FCAL.at(i).toElement();
        dx = getDataFloat ( node.elementsByTagName("dx").at(0) );
        dy = getDataFloat ( node.elementsByTagName("dy").at(0) );
        energy = getDataFloat ( node.elementsByTagName("energy").at(0) );
        id = getDataInt ( node.elementsByTagName("id").at(0) );
        layer = getDataInt ( node.elementsByTagName("layer").at(0) );
        sub = getDataInt ( node.elementsByTagName("sub").at(0) );
        x = getDataFloat ( node.elementsByTagName("x").at(0) );
        y = getDataFloat ( node.elementsByTagName("y").at(0) );
    }

    for (unsigned int i = 0; i < energy.size(); i++ )
    {
        if ( i<dx.size() ) s.dx = dx[i];
        if ( i<dy.size() ) s.dy = dy[i];
        s.energy = energy[i];
        if ( i<id.size() ) s.trackID = id[i];
        if ( i<layer.size() ) s.layer = layer[i];
        if ( i<sub.size() ) s.sub = sub[i];
        if ( i<x.size() ) s.x = x[i];
        if ( i<y.size() ) s.y = y[i];
        showers.push_back ( s );
    }

    return showers;
}

AEvent AXmlEvent::GetEventFromFile ( QString filename )
{
    //Base->message->setText( "<font color=\"red\">LOADING EVENT(S)</font>"  );
    Base->message->show();

    AEvent* e = new AEvent();

    QDomDocument doc("metainfo");
    QFile fh(filename);
    if (!fh.open(QIODevice::ReadOnly))
        return *e;
    if (!doc.setContent(&fh))
    {
        fh.close();
        return *e;
    }
    fh.close();
    highestTrackID = 0; //reset this to have consistent IDs every time we load an event

    e->STracks = GetSTracksFromDOM ( doc, e );
    e->Jets = GetJetsFromDOM (doc , e);
    e->MisET = GetMisETFromDOM (doc, e);
    e->LArshowers = GetShowersFromDOM ( doc, "LAr" );
    e->HECshowers = GetShowersFromDOM ( doc, "HEC" );
    e->TILEshowers = GetShowersFromDOM ( doc, "TILE" );
    e->FCALshowers = GetFCALShowersFromDOM ( doc );
    e->numTracks = 0;
    e->numChargedHadrons = 0;
    e->numPhotons = 0;
    e->numNeutralHadrons = 0;
    e->numNeutrinos = 0;
    e->numMuons = 0;
    e->numElectrons = 0;
    e->numShowers = e->LArshowers.size() + e->FCALshowers.size() + e->HECshowers.size() + e->TILEshowers.size();

    for ( vector<ASTrack*>::iterator go = e->STracks.begin(); go!= e->STracks.end(); go++ )
    {
        //cout << "\n In the selection, this track is of type " << go->Type ;
        //cout << "\n The track name is " << go->name;
        (*go)->node=0;
        if ( (*go)->Type == (*go)->eSTrack )
        {


            e->numTracks++;
            if ( (*go)->name == "gamma" ) e->numPhotons++;
            else if ( (*go)->name == "nu_e" || (*go)->name == "nu_mu" || (*go)->name == "nu_tau" || (*go)->name == "nu_ebar" || (*go)->name == "nu_mubar" || (*go)->name == "nu_taubar" ) e->numNeutrinos++;
            else if ( (*go)->name == "mu-" || (*go)->name == "mu+" ) e->numMuons++;
            else if ( (*go)->name == "e-" || (*go)->name == "e+" ) e->numElectrons++;
            else
            {
                if ( (*go)->q == 0 ) e->numNeutralHadrons++;
                else e->numChargedHadrons++;
            }
        }
    }


    bool inCaloETMis = false;
    bool inETMis = false;
    vector<float> v;

    QDomNodeList CaloETMis=doc.elementsByTagName("CaloETMis");

    for (unsigned int i=0;i<CaloETMis.length();i++)
    {
        QDomElement node=CaloETMis.at(i).toElement();

        v = getDataFloat ( node.elementsByTagName("et").at(0) );
        e->CaloETMis = v[0];
        v = getDataFloat ( node.elementsByTagName("etx").at(0) );
        e->CaloETMis = v[0];
        v = getDataFloat ( node.elementsByTagName("ety").at(0) );
        e->CaloETMis = v[0];
    }

    Base->message->hide();
    return *e;

}

void AXmlEvent::HideAllTracks()
{
    for ( vector<ATrack*>::iterator go = Event.Tracks.begin(); go!=Event.Tracks.end(); go++ )
        switch ( (*go)->Type )
        {
        case 1:
            (*go)->node->setTrackStyle ( 5 );
            break;
        case 2:
            (*go)->node->setTrackStyle ( 8 );
            break;
        }
}


void AXmlEvent::PtCutoff ( int PtCutInt )
{
    ptcut=PtCutInt;
    float PtCut=((float)PtCutInt)/1000;
    vector<ATrack*> NewEventTracks;
    vector<ASTrack*> NewEventSTracks;
    Event.numTracks = 0;
    Event.numChargedHadrons = 0;
    Event.numPhotons = 0;
    Event.numNeutralHadrons = 0;
    Event.numNeutrinos = 0;
    Event.numMuons = 0;
    Event.numElectrons = 0;
    Event.numShowers = EventComplete.FCALshowers.size() + EventComplete.HECshowers.size() + EventComplete.LArshowers.size() + EventComplete.TILEshowers.size();
    unsigned int j = 0;

    //This is a first approach to replace the old code. We'll treat the tracks by type, using the pointer vectors
    //First STracks
    for ( vector<ASTrack*>::iterator it = EventComplete.STracks.begin(); it!=EventComplete.STracks.end(); it++ )
    {
        ASTrack* go = *it;


        if (go->node) go->node->setTrackStyle ( 5 );
        if ( go->pt >= PtCut )
        {
            qDebug() << "cutoff STrack. Type = "<< go->Type << " eta = " << go->eta << " phi = " << go->phi << " Pt = " << go->pt << " code = " << go->code ;
            NewEventTracks.push_back ( go );
            NewEventSTracks.push_back ( go );

            if (go->node) go->node->setTrackStyle ( go->node->style );


            //Set visibility according to the checkbox states

            if ( go->Type == go->eSTrack )
            {
                if (go->node != NULL) go->node->setVisible ( false );
                for ( j = 0; j < P_checkbox_states.size(); j++ )
                {
                    if ( P_checkbox_states[j] )
                    {
                        switch ( j )
                        {

                        case 0: //electrons
                            if ( go->code == 11 || go->code == -11 )
                            {
                                Event.numTracks++;
                                Event.numElectrons++;
                                if (go->node) go->node->setVisible ( true );
                            }
                            break;

                        case 1: //muons
                            if ( abs ( go->code ) == 13 )
                            {
                                Event.numTracks++;
                                Event.numMuons++;
                                if (go->node) go->node->setVisible ( true );
                            }
                            break;

                        case 2: //photons
                            if ( go->code == 22 )
                            {
                                Event.numTracks++;
                                Event.numPhotons++;
                                if (go->node) go->node->setVisible ( true );
                            }
                            break;

                        case 3: //neutrinos
                            if ( abs ( go->code==12 ) || abs ( go->code==14 ) || abs ( go->code==16 ) )
                            {
                                Event.numTracks++;
                                Event.numNeutrinos++;
                                if (go->node) go->node->setVisible ( true );
                            }
                            break;
                        case 4: //neutral hadrons
                            if ( go->code==23 || go->code==111 || go->code==113 || go->code==130 ||
                                    go->code==221 || go->code==310 || abs ( go->code==421 ) || abs ( go->code==2212 ) ||
                                    abs ( go->code==3122 ) || abs ( go->code==3212 ) || abs ( go->code==3322 ) )
                            {
                                Event.numTracks++;
                                Event.numNeutralHadrons++;
                                if (go->node) go->node->setVisible ( true );
                            }
                            break;
                        case 5: //charged hadrons
                            if ( abs ( go->code== 15 ) || abs ( go->code== 24 ) || abs ( go->code== 211 ) ||
                                    abs ( go->code== 321 ) || abs ( go->code== 411 ) || abs ( go->code== 431 ) || abs ( go->code== 2212 ) ||
                                    abs ( go->code== 3112 ) || abs ( go->code== 3222 ) || abs ( go->code== 3312 ) || abs ( go->code== 3334 ) ||
                                    abs ( go->code== 4122 ) )
                            {
                                Event.numTracks++;
                                Event.numChargedHadrons++;
                                if (go->node) go->node->setVisible ( true );
                            }
                            break;

                        }
                    }
                }
            }
        }
    } // End of STracks selection

    qDebug() << "Ended PtCutoff() for STracks";

    //Now AJets
    for ( vector<AJet*>::iterator it = EventComplete.Jets.begin(); it!=EventComplete.Jets.end(); it++ )
    {
        AJet* go = *it;

        if (go->jtype == currentJetType )  // This adds the Jet to the vector "Tracks" in the event if it is of the right type
        {
            NewEventTracks.push_back ( go );
        }

    }
    qDebug() << "Ended PtCutoff() for Jets";

    // And Missing Et
    for ( vector<AMisET*>::iterator it = EventComplete.MisET.begin(); it!=EventComplete.MisET.end(); it++ )
    {
        AMisET* go = *it;
        if (go->node != NULL) go->node->setVisible ( false );
        if (go->mtype == currentMisEtType) // This adds Missing Et to the vector "Tracks" in the event if it is of the right type
        {
            if (go->node != NULL) go->node->setVisible ( true );
            NewEventTracks.push_back ( go );
        }
    }

    Event.Tracks =  NewEventTracks;
    Event.STracks =  NewEventSTracks;
    Event.Jets =  EventComplete.Jets;
    Event.MisET =  EventComplete.MisET;

    emit eventChanged();
    qDebug() << "Ended Pt Cutoff";
}

void AXmlEvent::setCurrentJetModel(QString jetType)
{
    if (jetType == QString("Kt4H1TowerJets"))
    {
        currentJetType = AJet::jKt4H1TowerJets;
    }
    else if (jetType == QString("Kt4H1TopoJets"))
    {
        currentJetType = AJet::jKt4H1TopoJets;
    }
    else if (jetType == QString("Cone4H1TowerJets"))
    {
        currentJetType = AJet::jCone4H1TowerJets;
    }
    else if (jetType == QString("Cone4H1TopoJets"))
    {
        currentJetType = AJet::jCone4H1TopoJets;
    }
}

void AXmlEvent::setCurrentTrackModel(QString trackType)
{
    //To be defined later. Once we have simulated data
}

void AXmlEvent::setCurrentMisEtModel(QString metType)
{
    if (metType == QString("mMET_Final"))
    {
        currentMisEtType = AMisET::mMET_Final;
    }
    else if (metType == QString("mMET_RefMuon"))
    {
        currentMisEtType = AMisET::mMET_RefMuon;
    }
    else if (metType == QString("mMET_Calib"))
    {
        currentMisEtType = AMisET::mMET_Calib;
    }
    else if (metType == QString("mMET_RefFinal"))
    {
        currentMisEtType = AMisET::mMET_RefFinal;
    }
    else if (metType == QString("mMET_Truth"))
    {
        currentMisEtType = AMisET::mMET_Truth;
    }
}

AEvent AXmlEvent::DisplayParticles ()
{
    if ( Event.Tracks.size() >1 )
    {
        Event.numTracks = 0;
        Event.numChargedHadrons = 0;
        Event.numPhotons = 0;
        Event.numNeutralHadrons = 0;
        Event.numNeutrinos = 0;
        Event.numMuons = 0;
        Event.numElectrons = 0;
        Event.numShowers = Event.FCALshowers.size() + Event.HECshowers.size() + Event.LArshowers.size() + Event.TILEshowers.size();


        // We do this in parts. First, STracks
        vector<ASTrack*>::iterator ii;
        unsigned int j = 0;


        for ( ii = Event.STracks.begin(); ii!=Event.STracks.end(); ii++ )
        {
            ASTrack* i = *ii;
            if ( i->Type == i->eSTrack )
            {
                i->node->setVisible ( false );
                for ( j = 0; j < P_checkbox_states.size(); j++ )
                {
                    if ( P_checkbox_states[j] )
                    {
                        switch ( j )
                        {

                        case 0: //electrons
                            if ( i->code == 11 || i->code == -11 )
                            {
                                Event.numTracks++;
                                Event.numElectrons++;
                                i->node->setVisible ( true );
                            }
                            break;

                        case 1: //muons
                            if ( abs ( i->code ) == 13 )
                            {
                                Event.numTracks++;
                                Event.numMuons++;
                                i->node->setVisible ( true );
                            }
                            break;

                        case 2: //photons
                            if ( i->code == 22 )
                            {
                                Event.numTracks++;
                                Event.numPhotons++;
                                i->node->setVisible ( true );
                            }
                            break;

                        case 3: //neutrinos
                            if ( abs ( i->code==12 ) || abs ( i->code==14 ) || abs ( i->code==16 ) )
                            {
                                Event.numTracks++;
                                Event.numNeutrinos++;
                                i->node->setVisible ( true );
                            }
                            break;
                        case 4: //neutral hadrons
                            if ( i->code==23 || i->code==111 || i->code==113 || i->code==130 ||
                                    i->code==221 || i->code==310 || abs ( i->code==421 ) || abs ( i->code==2212 ) ||
                                    abs ( i->code==3122 ) || abs ( i->code==3212 ) || abs ( i->code==3322 ) )
                            {
                                Event.numTracks++;
                                Event.numNeutralHadrons++;
                                i->node->setVisible ( true );
                            }
                            break;
                        case 5: //charged hadrons
                            if ( abs ( i->code== 15 ) || abs ( i->code== 24 ) || abs ( i->code== 211 ) ||
                                    abs ( i->code== 321 ) || abs ( i->code== 411 ) || abs ( i->code== 431 ) || abs ( i->code== 2212 ) ||
                                    abs ( i->code== 3112 ) || abs ( i->code== 3222 ) || abs ( i->code== 3312 ) || abs ( i->code== 3334 ) ||
                                    abs ( i->code== 4122 ) )
                            {
                                Event.numTracks++;
                                Event.numChargedHadrons++;
                                i->node->setVisible ( true );
                            }
                            break;
                        }
                    }
                }
            }
        }

        //Then AJets
        vector<AJet*>::iterator kk;

        for ( kk = Event.Jets.begin(); kk!=Event.Jets.end(); kk++ )
        {
            AJet* k = *kk;
            if ( k->Type == k->eJet )
            {
                k->node->setVisible ( false );
                if ( P_checkbox_states[6] )
                {
                    if ( k->Type == 2 && k->jtype == currentJetType )  // This shows the Jet in the event if it is of the right type
                    {
                        k->node->setVisible ( true );
                    }
                }
            }
        }

        //And Missing ET
        vector<AMisET*>::iterator mm;
        for ( mm = Event.MisET.begin(); mm!=Event.MisET.end(); mm++ )
        {
            AMisET* m = *mm;
            m->node->setVisible ( false );
            if ( m->Type == m->eMissingEt )
            {
                if ( P_checkbox_states[7] )
                {
                    if ( m->Type == 4 && m->mtype == currentMisEtType )
                    {
                        m->node->setVisible ( true );
                    }
                }
            }
        }
    }

    emit eventChanged();
    return Event;
}


void AXmlEvent::UnloadEvent()
{
    ISceneManager *smgr=Base->GetSceneManager();
    for ( vector<ATrack*>::iterator iter = EventComplete.Tracks.begin(); iter!=EventComplete.Tracks.end(); iter++ )
    {
        if ((*iter)->node)
        {
            (*iter)->node->remove();
        }
        delete *iter;
    }
    EventComplete.Tracks.clear();
}

void AXmlEvent::LoadEvent ( QString file )
{
    //if ( Base->GetTourBuilder() ) Base->GetTourBuilder()->markLoadEvent ( cstr_to_wstr ( ( char* ) file, strlen ( file ) ) );
    core::stringc filename ( file.toAscii().data() );
    core::stringc extension;
    core::getFileNameExtension ( extension, filename );
    extension.make_lower();

    if ( extension == ".xml" )
    {
        EventComplete = GetEventFromFile ( file );
        qDebug()<< "Total number of Tracks: " << EventComplete.Tracks.size();
        qDebug()<< "Total number of STracks: " << EventComplete.STracks.size();
        qDebug()<< "Total number of Jets: " << EventComplete.Jets.size();
        PtCutoff(ptcut);
    }
}

void AXmlEvent::DisplayEvent(AGeometry* device)
{
    for ( vector<ATrack*>::iterator iiter = EventComplete.Tracks.begin(); iiter != EventComplete.Tracks.end(); iiter++ )
    {
        ATrack* iter = *iiter;
        iter->node=0;
        iter->createTrackStructure( device->GetSceneManager()->getRootSceneNode(), device, 0 );
    }

}
