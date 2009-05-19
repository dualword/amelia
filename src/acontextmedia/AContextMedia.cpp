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

#include "AContextMedia.h"

#include <QDebug>
#include <QPushButton>

AContextMedia::AContextMedia( QObject *parent )
  : QObject(parent)
{
  headers["Muons_Reference"]="The Muon Spectrometer";
  text["Muons_Reference"]="The muon spectrometer consists of thousands of charged particle sensors placed in the magnetic field produced by large superconducting toroidal coils. The sensors are similar to the straws in the inner detector, but with larger tube diameters.";
  
  headers["TRT_Reference"]="The Transition Radiation Tracker";
  text["TRT_Reference"]="The TRT is based on the use of small diameter straw detectors that detect particles that ionize the gas around thin wires.";
  
  headers["Pixels_Reference"]="The Pixel Detector";
  text["Pixels_Reference"]="The pixel detector provides a very high granularity, high precision set of measurements as close to the interaction point as possible.";
  
  headers["SCT_Reference"]="The SCT Detector";
  text["SCT_Reference"]="The SCT system is designed to provide eight precision measurements per track in the intermediate outward range, contributing to the measurement of momentum, impact parameter and position.";

  headers["EMC_Reference"]="The Electromagnetic Calorimeter";
  text["EMC_Reference"]="The EM calorimeter is filled with liquid argon. The showers in the argon liberate electrons that are collected and recorded.";
  
  headers["TC_Reference"]="The Hadronic Calorimeter";
  text["TC_Reference"]="The hadronic calorimeter is made up of tiles of scintillating plastic. The showers cause the plastic to emit light which is then detected and recorded.";
  
  headers["Magnets_Reference"]="The Magnets";
  text["Magnets_Reference"]="The ATLAS detector is surrounded by 8 toroid magnets housed in separate cryostats and two endcap cryostats housing eight coils each. There is also a central solenoid with a length of 5.3 m with a bore of 2.4 m and is capable of producint fields ranging from 2 tesla to 2.6 tesla.";
}

AContextMedia::~AContextMedia()
{ }

void AContextMedia::load()
{ 
  geo=(AGeoPlugin *)AMELIA::global->plugin("AGeometry");
  
  QWidget *ageometry=geo->findWidget("Geometry");
  
  // Prepare info selections
  connect(ageometry,SIGNAL(detectorPartSelected(const QString&)),
	  this,SLOT(handleDetectorSelected(const QString&)));
  
  // Prepare movie menus
  QList<QString> partNames=headers.keys();
  for(int i=0;i<partNames.size();i++)
    {
      QAction *action=new QAction("Play Movie",this);
      videoMapper.setMapping(action,partNames[i]);
      connect(action,SIGNAL(triggered()),
	      &videoMapper,SLOT(map()));
      
      geo->addToDetectorMenu(partNames[i],action);
    }
  connect(&videoMapper,SIGNAL(mapped(const QString&)),
	  this,SLOT(playVideo(const QString&)));

}

void AContextMedia::handleDetectorSelected(const QString& partName)
{
  QPixmap icon(":/media/"+partName+".png");

  if(text.contains(partName))
     geo->displayMessage(text[partName],headers[partName],icon);
}

void AContextMedia::playVideo(const QString& partName)
{
  // Prepare the video playing widget
  QWidget *playerWdg=new QWidget();
  QLayout *playerLayout=new QVBoxLayout(playerWdg);
  
  Phonon::VideoPlayer *player=new Phonon::VideoPlayer(playerWdg);
  playerLayout->addWidget(player);

  AMainViewTmpWidget *tmp=geo->addMainViewTmpWidget(playerWdg);
  connect(player, SIGNAL(finished()), tmp, SLOT(close()));

  QPushButton *b1=new QPushButton();
  playerLayout->addWidget(b1);
  b1->setText("Close");
  QPushButton *b2=new QPushButton();
  b2->setText("Play");
  playerLayout->addWidget(b2);
  
  connect(b1,SIGNAL(clicked()),
	  tmp,SLOT(close()));
  connect(b2,SIGNAL(clicked()),
	  player,SLOT(play()));

  player->load(Phonon::MediaSource("media/"+partName+".mov"));
}

Q_EXPORT_PLUGIN2(AContextMedia, AContextMedia)
