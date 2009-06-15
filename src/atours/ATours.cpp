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

#include "ATours.h"

#include "ATourAction.h"
#include "ATourCameraAction.h"
#include "ATourMessageAction.h"
#include "ATourMonitorAction.h"
#include "ATourLoadEventAction.h"
#include "ATourPtCutAction.h"
#include "ATourMouseMoveAction.h"
#include "ATourViewportChangeAction.h"
#include "config.h"

#include <QDebug>
#include <QFile>
#include <QApplication>

ATours::ATours( QObject *parent )
  :QObject(parent)
{
  ATourAction::addActionType(ATourCameraAction::staticMetaObject); 
  ATourAction::addActionType(ATourMessageAction::staticMetaObject); 
  ATourAction::addActionType(ATourMonitorAction::staticMetaObject); 
  ATourAction::addActionType(ATourLoadEventAction::staticMetaObject); 
  ATourAction::addActionType(ATourPtCutAction::staticMetaObject); 
  ATourAction::addActionType(ATourMouseMoveAction::staticMetaObject); 
  ATourAction::addActionType(ATourViewportChangeAction::staticMetaObject); 
}

ATours::~ATours()
{ }

void ATours::load()
{
  geoplugin=(AGeoPlugin *)AMELIA::global->plugin("AGeometry");

  geo=(AGeometry*)geoplugin->findWidget("Geometry");
  QMenu *menu=(QMenu*)geoplugin->findWidget("menubar");
  QWidget *layerGUI=(QWidget*)geoplugin->findWidget("LayerGUI");


  
  toursMenu=menu->addMenu("Tours");
  toursListMenu=toursMenu->addMenu("List");

  tourPlayer.setupElements();

  prepareTours();
}

void ATours::prepareTours()
{
  connect(&tourMapper,SIGNAL(mapped(int)),
	  this,SLOT(startTour(int)));

  if (QFile::exists("media/tours"))
    tourManager.listTours ("media/tours");
  else
    tourManager.listTours (TOURS_PREFIX);
  
  for (int i = 0; i < tourManager.tourCount(); i++)
    {
      QAction *action=toursListMenu->addAction(tourManager.tour(i)->title());
      tourMapper.setMapping(action,i);
      connect(action,SIGNAL(triggered()),
	      &tourMapper,SLOT(map()));
    }
	 
}

void ATours::startTour(int selIndex)
{
  ATour *tour=tourManager.tour(selIndex);
  tourPlayer.play(tour);
}

Q_EXPORT_PLUGIN2(ATours, ATours)
