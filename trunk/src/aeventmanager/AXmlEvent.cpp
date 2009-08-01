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
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QString>

#include <string>
#include <vector>

#include "AEventPackage.h"

QMap<QString,AXmlEvent*> AXmlEvent::cache=QMap<QString,AXmlEvent*>();


AXmlEvent* AXmlEvent::CachedEvent(QString file)
{
    if (cache.contains(file))
        return cache[file];

    AXmlEvent* event=new AXmlEvent(file);
    event->LoadEvent();
    cache[file]=event;
    return event;
}


AXmlEvent::AXmlEvent(QString file):isLoaded(false)
{
    QFileInfo fileInfo(file);
    location=fileInfo.filePath();
    filename=fileInfo.fileName();
    setName(fileInfo.baseName());
}

AXmlEvent::~AXmlEvent()
{}

QList<int> AXmlEvent::getDataInt ( QDomNode xml )
{
    std::string sb;
    std::string add;
    QList<int> v;
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
            v.push_back ( sbi );
            sb = "";
        }
    }

    return v;
}

QList<float> AXmlEvent::getDataFloat ( QDomNode xml )
{
    std::string sb;
    std::string add;
    QList<float> v;
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
            v.push_back ( sbf );
            sb = "";
        }
    }

    return v;
}

void AXmlEvent::GetJetsFromDOM ( QDomDocument dom )
{
    // Get all the jet data from the XML file. There are four main jet types of interest, expressed as attributes in the file
    // Types are Kt4H1TopoJets, Cone4H1TopoJets, Kt4H1TowerJets and Cone4H1TowerJets

    //elements of the Jets
    QList<float> eta;
    QList<float> phi;
    QList<float> et;
    QList<float> numCells;


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
        for ( int s = 0; s < et.size(); s++ )
        {
            AJet* j = new AJet();
            j->et = et[s];
            j->setPt(j->et);
            if ( s<eta.size() ) j->eta = eta[s];
            if ( s<phi.size() ) j->phi = phi[s];
            if ( s<numCells.size() ) j->numCells = numCells[s];

            j->setJetType(attr.value());
            j->setTrackID(++highestTrackID);
            addTrack(j);
        }

    }
}

void AXmlEvent::GetMisETFromDOM ( QDomDocument dom )
{
    // Get all the Missing ET data from the XML file. There are five main types of MisET, expressed as attributes in the file
    // Types are MET_Final, MET_RefMuon, MET_Calib, MET_RefFinal

    //elements of MisET
    QList<float> etx;
    QList<float> ety;
    QList<float> et;


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
        for ( int s = 0; s < et.size(); s++ )
        {
            AMisET* m = new AMisET();
            m->et = et[s];
            m->setPt(m->et);
            if ( s<etx.size() ) m->etx = etx[s];
            if ( s<ety.size() ) m->ety = ety[s];

            m->setMisETType(attr.value());

            addTrack(m);
        }


    }
}

void AXmlEvent::GetSTracksFromDOM ( QDomDocument dom )
{
    // Aux variables and definitions

    //elements of the simulated tracks
    QList<int> str_code;
    QList<float> str_eta;
    QList<int> str_id;
    QList<float> str_phi;
    QList<float> str_phiVertex;
    QList<float> str_pt;
    QList<float> str_rhoVertex;
    QList<float> str_zVertex;
    QList<float> str_q;

    //Get the list of different types of simulated tracks
    QDomNodeList STr=dom.elementsByTagName("STr");

    //Load simulated tracks
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

        // Tracks section
        for ( int j = 0; j < str_code.size(); j++ )
        {

            ASTrack* t = new ASTrack();

            if ( j<str_code.size() ) t->setCode(str_code[j]);
            if ( j<str_eta.size() ) t->eta = str_eta[j];
            if ( j<str_id.size() ) t->setTrackID(str_id[j]);
            if ( j<str_phi.size() ) t->phi = str_phi[j];
            if ( j<str_phiVertex.size() ) t->phiVertex = str_phiVertex[j];
            if ( j<str_pt.size() ) t->setPt(str_pt[j]);
            if ( j<str_rhoVertex.size() ) t->rhoVertex = str_rhoVertex[j];
            if ( j<str_zVertex.size() ) t->zVertex = str_zVertex[j];

            addTrack(t);
        }
    }
}

void AXmlEvent::GetRTracksFromDOM ( QDomDocument dom )
{

    QList<int> rtr_barcode;
    QList<float> rtr_chi2;
    QList<int> rtr_id;
    QList<float> rtr_cotTheta;
    QList<float> rtr_d0;
    QList<float> rtr_pt;
    QList<float> rtr_phi0;
    QList<float> rtr_z0;
    //Now for the polyline
    QList<int> rtr_numPolyline;
    QList<float> rtr_polylineX;
    QList<float> rtr_polylineY;
    QList<float> rtr_polylineZ;


    //Get the list of different types of "tracks"
    QDomNodeList RTracks=dom.elementsByTagName("Tracks");

    //Load the cool tracks!
    for (unsigned int i=0;i<RTracks.length();i++)
    {
        QDomElement node=RTracks.at(i).toElement();
        QDomAttr attr = node.attributeNode( "storeGateKey" );


        rtr_barcode = getDataInt ( node.elementsByTagName("barcode").at(0) );
        rtr_chi2 = getDataFloat ( node.elementsByTagName("chi2").at(0) );
        rtr_id = getDataInt ( node.elementsByTagName("id").at(0) );
        rtr_cotTheta = getDataFloat ( node.elementsByTagName("cotTheta").at(0) );
        rtr_d0 = getDataFloat ( node.elementsByTagName("d0").at(0) );
        rtr_pt = getDataFloat ( node.elementsByTagName("pt").at(0) );
        rtr_phi0 = getDataFloat ( node.elementsByTagName("phi0").at(0) );
        rtr_z0 = getDataFloat ( node.elementsByTagName("z0").at(0) );
        rtr_numPolyline = getDataInt ( node.elementsByTagName("numPolyline").at(0) );
        rtr_polylineX = getDataFloat ( node.elementsByTagName("polylineX").at(0) );
        rtr_polylineY = getDataFloat ( node.elementsByTagName("polylineY").at(0) );
        rtr_polylineZ = getDataFloat ( node.elementsByTagName("polylineZ").at(0) );
        int bookmarkPolyline = 0; //just a little control variable to navigate trough the polyline data


        // Tracks section
        for ( int j = 0; j < rtr_barcode.size(); j++ )
        {

            ARTrack* t = new ARTrack();

            if ( j<rtr_barcode.size() ) t->barcode = rtr_barcode[j];
            if ( j<rtr_chi2.size() ) t->chi2 = rtr_chi2[j];
            if ( j<rtr_id.size() ) t->setTrackID(rtr_id[j]);
            if ( j<rtr_cotTheta.size() ) t->cotTheta = rtr_cotTheta[j];
            if ( j<rtr_d0.size() ) t->d0 = rtr_d0[j];
            if ( j<rtr_pt.size() ) t->setPt(rtr_pt[j]);
            if ( j<rtr_z0.size() ) t->z0 = rtr_z0[j];

            // Now for the polyline part
            if ( j<rtr_numPolyline.size() )
            {
                int polySections = rtr_numPolyline[j];
                for ( int p = bookmarkPolyline; j < bookmarkPolyline + polySections; j++ )
                {
                    irr::core::vector3df polyPoint = irr::core::vector3df (rtr_polylineX[p], rtr_polylineY[p], rtr_polylineZ[p]  );
                    t->polyLine.push_back ( polyPoint );
                }
                bookmarkPolyline += polySections;
            }

            t->setRTrackType(attr.value());
            addTrack(t);

        }
    }
}



void AXmlEvent::GetShowersFromDOM ( QDomDocument dom, QString calo )
{
    //gets data for all HEC, LAr, and TILE showers in event; arguments are xml file name, device, and calorimeter type
    QList<float> energy;
    QList<float> eta;
    QList<int> id;
    QList<int> layer;
    QList<float> phi;
    QList<int> sub;

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
        for (int j = 0; j < energy.size(); j++ )
        {
            AShower* s=new AShower();
            s->energy = energy[j];
            if ( j<eta.size() ) s->eta = eta[j];
            if ( j<id.size() ) s->setTrackID(id[j]);
            if ( j<layer.size() ) s->layer = layer[j];
            if ( j<phi.size() ) s->phi = phi[j];
            if ( j<sub.size() ) s->sub = sub[j];
            s->calometer=calo;

            addTrack(s);
        }
    }
}

void AXmlEvent::GetFCALShowersFromDOM ( QDomDocument dom )
{
    //gets data for all HEC showers in event

    AFCALShower* s;

    QList<float> dx;
    QList<float> dy;
    QList<float> energy;
    QList<int> id;
    QList<int> layer;
    QList<int> sub;
    QList<float> x;
    QList<float> y;

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

        for (int j = 0; j < energy.size(); j++ )
        {
            s=new AFCALShower();
            if ( j<dx.size() ) s->dx = dx[j];
            if ( j<dy.size() ) s->dy = dy[j];
            s->energy = energy[j];
            if ( j<id.size() ) s->setTrackID(id[j]);
            if ( j<layer.size() ) s->layer = layer[j];
            if ( j<sub.size() ) s->sub = sub[j];
            if ( j<x.size() ) s->x = x[j];
            if ( j<y.size() ) s->y = y[j];

            addTrack(s);
        }
    }

}
#include <QDir>
void AXmlEvent::GetEventFromFile ( QString filename )
{
    qDebug() << "Event from file " << filename;
    QDomDocument doc("metainfo");
    QFile fh(filename);

    if (!fh.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&fh))
    {
        fh.close();
        return;
    }
    fh.close();

    QDomNodeList eventTags=doc.elementsByTagName("Event");
    runNumber=eventTags.at(0).attributes().namedItem("runNumber").toAttr().value().toInt();
    eventNumber=eventTags.at(0).attributes().namedItem("eventNumber").toAttr().value().toInt();

    GetSTracksFromDOM ( doc );
    GetRTracksFromDOM ( doc );
    GetJetsFromDOM (doc );
    GetMisETFromDOM (doc );
    GetShowersFromDOM ( doc, "LAr" );
    GetShowersFromDOM ( doc, "HEC" );
    GetShowersFromDOM ( doc, "TILE" );
    GetFCALShowersFromDOM ( doc );

    numShowers = LArshowers.size() + FCALshowers.size() + HECshowers.size() + TILEshowers.size();

    QList<float> v;

    QDomNodeList CaloETMis=doc.elementsByTagName("CaloETMis");

    /*for (unsigned int i=0;i<CaloETMis.length();i++)
      {
        QDomElement node=CaloETMis.at(i).toElement();

        v = getDataFloat ( node.elementsByTagName("et").at(0) );
        e->CaloETMis = v[0];
        v = getDataFloat ( node.elementsByTagName("etx").at(0) );
        e->CaloETMis = v[0];
        v = getDataFloat ( node.elementsByTagName("ety").at(0) );
        e->CaloETMis = v[0];
      }*/
}

void AXmlEvent::LoadEvent ( )
{
    //if ( locaiton.toLower().endsWith(".xml") )
    //{
    if (!isLoaded)
        GetEventFromFile (location);
    isLoaded=true;
    //}
}

void AXmlEvent::setPackage(AEventPackage *pkg)
{
    AEvent::setPackage(pkg);

    QFile file(location);
    QFileInfo fileInfo(location);
    QString currentLocation=location;

    QString newLocation;
    if (pkg==0) //If removing from a package, save the file to a temporary location
        newLocation=QDir::tempPath()+"/"+fileInfo.fileName();
    else
        newLocation=pkg->location()+"/"+fileInfo.fileName();

    if (currentLocation!=newLocation)
    {
        file.copy(newLocation);
        file.remove();
        location=newLocation;
    }
}
