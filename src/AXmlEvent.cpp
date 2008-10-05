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

int XmlEvent::ptcut=1000;

QMap<QString,XmlEvent*> XmlEvent::cache=QMap<QString,XmlEvent*>();


XmlEvent* XmlEvent::CachedEvent(QString file)
{
  if(cache.contains(file))
    return cache[file];

  XmlEvent* event=new XmlEvent();
  event->LoadEvent(file.toLocal8Bit().data());
  cache[file]=event;
  return event;
}


XmlEvent::XmlEvent()
{
    //Initialize the bool vector describing which type of particle is visible
    for (int i=0; i < 8 ; i++)
    {
        P_checkbox_states.push_back(true);
    }

}

XmlEvent::~XmlEvent()
{
}



std::vector<int> XmlEvent::getDataInt ( QDomNode xml )
{
    std::string sb;
    std::string add;
    vector<int>v;
    int sbi;

    QDomText textNode=xml.firstChild().toText();
    std::string dstr = textNode.data().trimmed().toLocal8Bit().data();
    
    for ( int i=0; i < dstr.size(); i++ )
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

std::vector<float> XmlEvent::getDataFloat ( QDomNode xml )
{
    std::string sb;
    std::string add;
    vector<float>v;
    float sbf;

    QDomText textNode=xml.firstChild().toText();
    std::string dstr = textNode.data().toLocal8Bit().data();
    
    for ( int i=0; i < dstr.size(); i++ )
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


std::vector <track> XmlEvent::GetTracksFromDOM ( QDomDocument dom )
{
  // Aux variables and definitions
  std::vector <track> tracks;
  int i;
  
  track t;
  track e;
  
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
  
  //elements of the Jets
  vector<float> jet_eta;
  vector<float> jet_phi;
  vector<float> jet_et;
  
  
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
  QDomNodeList Jet=dom.elementsByTagName("Jet");

  //Load the cool tracks!
  for(int i=0;i<STr.length();i++)
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

  // read the jet data and store it on the jet_ vectors
  for(int i=0;i<Jet.length();i++)
    {
      QDomElement node=Jet.at(i).toElement();
      jet_eta = getDataFloat ( node.elementsByTagName("eta").at(0) );
      jet_phi = getDataFloat ( node.elementsByTagName("phi").at(0) );
      jet_et = getDataFloat ( node.elementsByTagName("et").at(0) );
    }

  // read and write the ETMis data  
  for(int i=0;i<ETMis.length();i++)
    {
      QDomElement node=ETMis.at(i).toElement();
      
      v = getDataFloat ( node.elementsByTagName("et").at(0) );
      e.et = v[0];
      v = getDataFloat ( node.elementsByTagName("etx").at(0) );
      e.etx = v[0];
      v = getDataFloat ( node.elementsByTagName("ety").at(0) );
      e.ety = v[0];
    }
  
  /// Write the XML data to the tracks
  
  //Missing Energy
  e.Type = e.eMissingEt;
  e.name = "MissEt";
  tracks.push_back ( e );
  
  // Tracks section
  int j = 0;
  for ( i = 0; i < str_code.size(); i++ )
    {
      
      t.name = "unknown";
      for ( j = 0; j < 52; j++ )
        {
	  if ( codelist[j] == str_code[i] )
            {
	      t.q = chargelist[j];
	      t.name = namelist[j];
	      t.trackColor = colorlist[j];
            }
        }
      
      //if ( t.name == "unknown" ) cout<<"Unkown particle code: "<<str_code[i]<<endl;
      /*if ( t.name == "bananas" )
        {
	device->drop();
	cout<<"Banana"<<endl;
	cout<<"Split"<<endl;
        }*/
      
      if ( i<str_code.size() ) t.code = str_code[i];
      if ( i<str_eta.size() ) t.eta = str_eta[i];
      if ( i<str_id.size() ) t.trackID = str_id[i];
      if ( i<str_phi.size() ) t.phi = str_phi[i];
      if ( i<str_phiVertex.size() ) t.phiVertex = str_phiVertex[i];
      if ( i<str_pt.size() ) t.pt = str_pt[i];
      if ( i<str_rhoVertex.size() ) t.rhoVertex = str_rhoVertex[i];
      if ( i<str_zVertex.size() ) t.zVertex = str_zVertex[i];
      t.Type = t.eSTrack;
      tracks.push_back ( t );
    }
  
  
  // Jets section
  j = 0;
  for ( i = 0; i < jet_eta.size(); i++ )
    {
      t.name = "jet";
      if ( i<jet_eta.size() ) t.eta = jet_eta[i];
      if ( i<jet_phi.size() ) t.phi = jet_phi[i];
      if ( i<jet_et.size() ) t.et = jet_et[i];
      t.Type = t.eJet;
      tracks.push_back ( t );
    }
  
  return tracks;
}

std::vector <shower> XmlEvent::GetShowersFromDOM ( QDomDocument dom, char* calo )
{
    //gets data for all HEC, LAr, and TILE showers in event; arguments are xml file name, device, and calorimeter type

    std::vector <shower> showers;
    shower s;

    int i;

    vector<float> energy;
    vector<float> eta;
    vector<int> id;
    vector<int> layer;
    vector<float> phi;
    vector<int> sub;

    QDomNodeList Calo=dom.elementsByTagName(calo);

    //Load the calometers
    for(int i=0;i<Calo.length();i++)
      {
	QDomElement node=Calo.at(i).toElement();
	energy = getDataFloat ( node.elementsByTagName("energy").at(0) );
	eta = getDataFloat ( node.elementsByTagName("eta").at(0) );
	id = getDataInt ( node.elementsByTagName("id").at(0) );
	layer = getDataInt ( node.elementsByTagName("layer").at(0) );
	phi = getDataFloat ( node.elementsByTagName("phi").at(0) );
	sub = getDataInt ( node.elementsByTagName("sub").at(0) );
      }
    
    for ( i = 0; i < energy.size(); i++ )
    {
        s.energy = energy[i];
        if ( i<eta.size() ) s.eta = eta[i];
        if ( i<id.size() ) s.id = id[i];
        if ( i<layer.size() ) s.layer = layer[i];
        if ( i<phi.size() ) s.phi = phi[i];
        if ( i<sub.size() ) s.sub = sub[i];
        showers.push_back ( s );
    }

    return showers;
}

std::vector <FCALshower> XmlEvent::GetFCALShowersFromDOM ( QDomDocument dom )
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

    for(int i=0;i<FCAL.length();i++)
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
    
    for ( i = 0; i < energy.size(); i++ )
    {
        if ( i<dx.size() ) s.dx = dx[i];
        if ( i<dy.size() ) s.dy = dy[i];
        s.energy = energy[i];
        if ( i<id.size() ) s.id = id[i];
        if ( i<layer.size() ) s.layer = layer[i];
        if ( i<sub.size() ) s.sub = sub[i];
        if ( i<x.size() ) s.x = x[i];
        if ( i<y.size() ) s.y = y[i];
        showers.push_back ( s );
    }

    return showers;
}

Aevent XmlEvent::GetEventFromFile ( const char* filename )
{
  Aevent e;
  
  QDomDocument doc("metainfo");
  QFile fh(filename);
  if (!fh.open(QIODevice::ReadOnly))
    return e;
  if (!doc.setContent(&fh)) {
    fh.close();
    return e;
  }
  fh.close();
  
  e.tracks = GetTracksFromDOM ( doc );
  e.LArshowers = GetShowersFromDOM ( doc, "LAr" );
  e.HECshowers = GetShowersFromDOM ( doc, "HEC" );
  e.TILEshowers = GetShowersFromDOM ( doc, "TILE" );
  e.FCALshowers = GetFCALShowersFromDOM ( doc );
  e.numTracks = 0;
  e.numChargedHadrons = 0;
  e.numPhotons = 0;
  e.numNeutralHadrons = 0;
  e.numNeutrinos = 0;
  e.numMuons = 0;
  e.numElectrons = 0;
  e.numShowers = e.LArshowers.size() + e.FCALshowers.size() + e.HECshowers.size() + e.TILEshowers.size();
  for ( vector<track>::iterator go = e.tracks.begin(); go!=e.tracks.end(); go++ )
    {
      go->node=0;
      if ( go->Type == go->eSTrack )
        {
	  e.numTracks++;
	  if ( go->name == "gamma" ) e.numPhotons++;
	  else if ( go->name == "nu_e" || go->name == "nu_mu" || go->name == "nu_tau" || go->name == "nu_ebar" || go->name == "nu_mubar" || go->name == "nu_taubar" ) e.numNeutrinos++;
	  else if ( go->name == "mu-" || go->name == "mu+" ) e.numMuons++;
	  else if ( go->name == "e-" || go->name == "e+" ) e.numElectrons++;
	  else
            {
	      if ( go->q == 0 ) e.numNeutralHadrons++;
	      else e.numChargedHadrons++;
            }
        }
    }

  bool inCaloETMis = false;
  bool inETMis = false;
  vector<float> v;

  QDomNodeList CaloETMis=doc.elementsByTagName("CaloETMis");
  
  for(int i=0;i<CaloETMis.length();i++)
    {
      QDomElement node=CaloETMis.at(i).toElement();
      
      v = getDataFloat ( node.elementsByTagName("et").at(0) );
      e.CaloETMis = v[0];
      v = getDataFloat ( node.elementsByTagName("etx").at(0) );
      e.CaloETMis = v[0];
      v = getDataFloat ( node.elementsByTagName("ety").at(0) );
      e.CaloETMis = v[0];
    }
  
  return e;

}

void XmlEvent::HideAllTracks()
{
    for ( vector<track>::iterator go = Event.tracks.begin(); go!=Event.tracks.end(); go++ )
        switch ( go->Type )
        {
        case 1:
            go->node->setTrackStyle ( 5 );
            break;
        case 2:
            go->node->setTrackStyle ( 8 );
            break;
        }
}


void XmlEvent::PtCutoff ( int PtCutInt )
{
  ptcut=PtCutInt;
  float PtCut=((float)PtCutInt)/1000;
    vector<track> NewEventTracks;
    Event.numTracks = 0;
    Event.numChargedHadrons = 0;
    Event.numPhotons = 0;
    Event.numNeutralHadrons = 0;
    Event.numNeutrinos = 0;
    Event.numMuons = 0;
    Event.numElectrons = 0;
    Event.numShowers = EventComplete.FCALshowers.size() + EventComplete.HECshowers.size() + EventComplete.LArshowers.size() + EventComplete.TILEshowers.size();
    int j = 0;
    for ( vector<track>::iterator go = EventComplete.tracks.begin(); go!=EventComplete.tracks.end(); go++ )
    {
        if ( go->Type == go->eJet )
	  if(go->node) go->node->setTrackStyle ( 9 );

        if ( go->Type != go->eSTrack )
        {
            NewEventTracks.push_back ( *go );
        }
        else
        {
            if ( go->pt >= PtCut )
            {
                NewEventTracks.push_back ( *go );
		
		if(go->node) go->node->setTrackStyle ( go->node->style );

                //Set visibility according to the checkbox states

                if ( go->Type == go->eSTrack )
                {
		  if(go->node) go->node->setVisible ( false );
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
                                    if(go->node) go->node->setVisible ( true );
                                }
                                break;

                            case 1: //muons
                                if ( abs ( go->code ) == 13 )
                                {
                                    Event.numTracks++;
                                    Event.numMuons++;
				    				if(go->node) go->node->setVisible ( true );
                                }
                                break;

                            case 2: //photons
                                if ( go->code == 22 )
                                {
                                    Event.numTracks++;
                                    Event.numPhotons++;
				   					if(go->node) go->node->setVisible ( true );
                                }
                                break;

                            case 3: //neutrinos
                                if ( abs ( go->code==12 ) || abs ( go->code==14 ) || abs ( go->code==16 ) )
                                {
                                    Event.numTracks++;
                                    Event.numNeutrinos++;
				    				if(go->node) go->node->setVisible ( true );
                                }
                                break;
                            case 4: //neutral hadrons
                                if ( go->code==23 || go->code==111 || go->code==113 || go->code==130 ||
                                        go->code==221 || go->code==310 || abs ( go->code==421 ) || abs ( go->code==2212 ) ||
                                        abs ( go->code==3122 ) || abs ( go->code==3212 ) || abs ( go->code==3322 ) )
                                {
                                    Event.numTracks++;
                                    Event.numNeutralHadrons++;
				    				if(go->node) go->node->setVisible ( true );
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
				    				if(go->node) go->node->setVisible ( true );
                                }
                                break;

                            }
                        }
                    }
                }




            }
            else
            {
	      if(go->node) 
		{
		  go->node->setTrackStyle ( 0 );
		  go->node->setVisible ( false );
		}
            }
        }
    }
    Event.tracks =  NewEventTracks;
    //Event = NewEvent;
    emit eventChanged();
}



Aevent XmlEvent::DisplayParticles ( vector<bool>states, Aevent &ievent )
{
    if ( ievent.tracks.size() >1 )
    {
        ievent.numTracks = 0;
        ievent.numChargedHadrons = 0;
        ievent.numPhotons = 0;
        ievent.numNeutralHadrons = 0;
        ievent.numNeutrinos = 0;
        ievent.numMuons = 0;
        ievent.numElectrons = 0;
        ievent.numShowers = ievent.FCALshowers.size() + ievent.HECshowers.size() + ievent.LArshowers.size() + ievent.TILEshowers.size();

        vector<track>::iterator i;
        int j = 0;


        for ( i = ievent.tracks.begin(); i!=ievent.tracks.end(); i++ )
        {
            if ( i->Type == i->eSTrack )
            {
                i->node->setVisible ( false );
                for ( j = 0; j < states.size(); j++ )
                {
                    if ( states[j] )
                    {
                        switch ( j )
                        {

                        case 0: //electrons
                            if ( i->code == 11 || i->code == -11 )
                            {
                                ievent.numTracks++;
                                ievent.numElectrons++;
                                i->node->setVisible ( true );
                            }
                            break;

                        case 1: //muons
                            if ( abs ( i->code ) == 13 )
                            {
                                ievent.numTracks++;
                                ievent.numMuons++;
                                i->node->setVisible ( true );
                            }
                            break;

                        case 2: //photons
                            if ( i->code == 22 )
                            {
                                ievent.numTracks++;
                                ievent.numPhotons++;
                                i->node->setVisible ( true );
                            }
                            break;

                        case 3: //neutrinos
                            if ( abs ( i->code==12 ) || abs ( i->code==14 ) || abs ( i->code==16 ) )
                            {
                                ievent.numTracks++;
                                ievent.numNeutrinos++;
                                i->node->setVisible ( true );
                            }
                            break;
                        case 4: //neutral hadrons
                            if ( i->code==23 || i->code==111 || i->code==113 || i->code==130 ||
                                    i->code==221 || i->code==310 || abs ( i->code==421 ) || abs ( i->code==2212 ) ||
                                    abs ( i->code==3122 ) || abs ( i->code==3212 ) || abs ( i->code==3322 ) )
                            {
                                ievent.numTracks++;
                                ievent.numNeutralHadrons++;
                                i->node->setVisible ( true );
                            }
                            break;
                        case 5: //charged hadrons
                            if ( abs ( i->code== 15 ) || abs ( i->code== 24 ) || abs ( i->code== 211 ) ||
                                    abs ( i->code== 321 ) || abs ( i->code== 411 ) || abs ( i->code== 431 ) || abs ( i->code== 2212 ) ||
                                    abs ( i->code== 3112 ) || abs ( i->code== 3222 ) || abs ( i->code== 3312 ) || abs ( i->code== 3334 ) ||
                                    abs ( i->code== 4122 ) )
                            {
                                ievent.numTracks++;
                                ievent.numChargedHadrons++;
                                i->node->setVisible ( true );
                            }
                            break;

                        }
                    }
                }
            }

            if ( i->Type == i->eJet )
            {
                i->node->setVisible ( false );
                if ( states[6] )
                {
                    if ( i->Type == 2 )
                    {
                        i->node->setVisible ( true );
                    }
                }
            }

            if ( i->Type == i->eMissingEt )
            {
                i->node->setVisible ( false );
                if ( states[7] )
                {
                    if ( i->Type == 4 )
                    {
                        i->node->setVisible ( true );
                    }
                }
            }


        }
    }
    emit eventChanged();
    return ievent;
}

void XmlEvent::UnloadEvent()
{
  ISceneManager *smgr=Base->GetSceneManager();
  for ( vector<track>::iterator iter = EventComplete.tracks.begin(); iter!=EventComplete.tracks.end(); iter++ )
    if(iter->node)
      iter->node->remove();
  EventComplete.tracks.clear();
}

void XmlEvent::LoadEvent ( const c8* file )
{
  //if ( Base->GetTourBuilder() ) Base->GetTourBuilder()->markLoadEvent ( cstr_to_wstr ( ( char* ) file, strlen ( file ) ) );
  core::stringc filename ( file );
  core::stringc extension;
  core::getFileNameExtension ( extension, filename );
  extension.make_lower();
  
  if ( extension == ".xml" )
    {
      EventComplete = GetEventFromFile ( file );
      PtCutoff(ptcut);
    }
}

void XmlEvent::DisplayEvent(AGeometry* device)
{
  int jetIdCounter = 0;
      
  for ( vector<track>::iterator iter = EventComplete.tracks.begin(); iter != EventComplete.tracks.end(); iter++ )
    {
      iter->node=0;
      
      if ( iter->Type == 1 )
	{
	  HelixSceneNode* HelixNode = new HelixSceneNode ( device->GetSceneManager()->getRootSceneNode(), device, 0 );
	  iter->node = HelixNode;
	  iter->node->charge = iter->q;
	  iter->node->eta = iter->eta;
	  iter->node->phi = iter->phi;
	  iter->node->pt = iter->pt;
	  iter->et = iter->pt;
	  iter->node->v_phi = iter->phiVertex;
	  iter->node->v_rho = iter->rhoVertex;
	  iter->node->v_z = iter->zVertex;
	  iter->node->vividColor = iter->trackColor;
	  iter->node->calculateDimmedColors();
	  iter->node->setTrack ( &*iter );
	  iter->node->type = 1; //0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
	  iter->style = 1;
	  iter->node->style = 1;
	  
	  
	  if ( iter->q == 0 )
	    {
	      std::vector<core::vector3df> StartEndNeutral = HelixNode->getNeutralPath();
	      iter->start = StartEndNeutral.front();
	      iter->end = StartEndNeutral.back();
	      iter->node->start = iter->start;
	      iter->node->end = iter->end;
	      iter->node->curvePoints.push_back ( core::vector3df ( 0,0,0 ) );
	      //iter->node->createBoxesNeutral();
	      
	    }
	  else
	    {
	      iter->maxAngle = HelixNode->getChargedMaxAngle();
	      iter->node->maxAngle = iter->maxAngle;
	      iter->node->createCurveVector();
	      //iter->node->createBoxesCharged();
	      
	    }
	  
	  if ( ( iter->name == "mu-" || iter->name == "mu+" || iter->name == "e+" || iter->name == "e-" ) && iter->pt >= 2 )
	    {
	      iter->style = 1;
	    }
	  else
	    {
	      iter->style = 1;
	    }
	  
	  iter->node->boxMode = false;
	  iter->node->style = iter->style;
	  iter->node->setTrackStyle ( iter->style );
	}
      else if ( iter->Type == 2 ) // jets
	{
	  HelixSceneNode* HelixNode = new HelixSceneNode ( device->GetSceneManager()->getRootSceneNode(), device, 0 );
	  iter->node = HelixNode;
	  iter->node->eta = iter->eta;
	  iter->node->phi = iter->phi;
	  iter->node->isLineVisible = false;
	  iter->style = 7;
	  iter->selectionID == --jetIdCounter;
	  
	  iter->node->type = 2; //0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
	  iter->node->setTrack ( &*iter );
	  iter->node->createJetPyramids();
	}
      else if ( iter->Type == 4 ) //Missing Et
	{
	  HelixSceneNode* HelixNode = new HelixSceneNode ( device->GetSceneManager()->getRootSceneNode(), device, 0 );
	  iter->node = HelixNode;
	  iter->node->eta = iter->eta = 0;
	  iter->node->phi = iter->phi = 0;
	  iter->node->pt = iter->pt = iter->et;
	  iter->node->etx = iter->etx;
	  iter->node->ety = iter->ety;
	  iter->node->isLineVisible = false;
	  iter->style = 11;
	  
	  iter->node->type = 4; //0 = Undefined, 1 = STrack, 2 = Jet, 3 = Shower, 4 = Missing Energy
	  iter->node->setTrack ( &*iter );
	  iter->node->createMisEtBoxes();
	}
      else
	{
	  qDebug() << "Unknown " << iter->Type;
	}
      
      iter->tL = iter->node->tL;
      iter->Mlv = iter->node->Mlv;
      
      
      
      
      iter->selectionID = 0;
      iter->isInList = false;

    }
}
