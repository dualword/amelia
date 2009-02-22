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

#include "AHistPlugin.h"

#include <QMenu>
#include <QMainWindow>
#include <QInputDialog>

#include <abase/ABase.h>
#include <ageometry/AGeoPlugin.h>
#include <ageometry/QAbstractTableModelWithContextMenu.h>

#include "ATest.h"

#include "ui_allhistograms.h"

AHistPlugin::AHistPlugin(QObject *parent):QObject(parent)
{ }

AHistPlugin::~AHistPlugin()
{ }

void AHistPlugin::load()
{ 
  ABase *base=(ABase *)AMELIA::global->plugin("ABase");
  AGeoPlugin *geo=(AGeoPlugin *)AMELIA::global->plugin("AGeometry");
  

  ATest *test=new ATest;
  base->addLevel("hist","Analysis",test);


  combinedTracksTable=(QTableView*)geo->findWidget("combinedTracksTable"); 
  QAbstractTableModelWithContextMenu *model=(QAbstractTableModelWithContextMenu*)combinedTracksTable->model();
  QMenu *menu=model->menu();
  hists=new AllHistograms(menu);
  base->addLevel("combohist","Analysis",hists);
}

void AHistPlugin::showContextMenu(const QPoint& pos)
{
  //QMenu menu;

  //QStringList histList=hists->
  
  //menu.addSeparator();
  //menu.addAction("New Histogram",hists,SLOT(newHistorgamDlg()));

  //menu.exec(combinedTracksTable->mapToGlobal(pos));
}


Q_EXPORT_PLUGIN2(AHistPlugin, AHistPlugin)
