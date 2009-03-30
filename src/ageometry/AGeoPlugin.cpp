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

#include "AGeoPlugin.h"

#include "ui_geometry.h"
#include <abase/ABase.h>
#include <aeventmanager/AEventManager.h>

AGeoPlugin::AGeoPlugin( QObject *parent )
        : QObject(parent)
{
  comboMenu.setTitle("Combination");
  mainViewMapper.setParent(this);
}

AGeoPlugin::~AGeoPlugin() { }

void AGeoPlugin::load()
{
  AMELIA *app=pluginBase();
  ABase *base=(ABase *)app->plugin("ABase");

  //Setup the UI
  Ui::MainWindow geoUI;
  geoWin=new QMainWindow(base);
  geoUI.setupUi(geoWin);

  //Load the ALayerGUI
  ALayerGUI* _layerGUI=geoWin->findChild<ALayerGUI*>("LayerGUI");
  geoWin->setCentralWidget(_layerGUI);
  AEventManager *mngr=(AEventManager *)app->plugin("AEventManager");
  _layerGUI->setupElements(mngr);
  connect(_layerGUI,SIGNAL(eventLoaded(AEvent*)),
	  this,SLOT(handleNewEventLoaded(AEvent*)));

  //Setup the menu for the main view
  mainView=geoWin->findChild<QStackedWidget*>("AGeometryFrame");
  menuMain_View=geoWin->findChild<QMenu*>("menuMain_View");
  QAction* actionDetector=geoWin->findChild<QAction*>("actionDetector");
  QAction* actionTables=geoWin->findChild<QAction*>("actionTables");
  mainViewMapper.setMapping(actionDetector,0);
  mainViewMapper.setMapping(actionTables,1);
  connect(actionDetector,SIGNAL(triggered()),
	  &mainViewMapper,SLOT(map()));
  connect(actionTables,SIGNAL(triggered()),
	  &mainViewMapper,SLOT(map()));
  
  connect(&mainViewMapper,SIGNAL(mapped(int)),
	  mainView,SLOT(setCurrentIndex(int)));

  //Setup the menu for track combinations
  QTableView *combinedTracksTable= geoWin->findChild<QTableView*>("combinedTracksTable");
  QAbstractTableModelWithContextMenu *model=(QAbstractTableModelWithContextMenu*)combinedTracksTable->model();
  model->setMenu(&comboMenu);

  AGeometry *Geometry= geoWin->findChild<AGeometry*>("Geometry");
  Geometry->setComboMenu(&comboMenu);

  base->addMonitor("ageometry","Default",geoWin,"Enter ATLAS");
}

QWidget* AGeoPlugin::findWidget(QString name)
{
  return geoWin->findChild<QWidget*>(name);
}

QMenu* AGeoPlugin::addTrackComboMenu(QString text)
{
  return comboMenu.addMenu(text);
}

void AGeoPlugin::addMainViewWidget(QWidget* widget,QString title)
{
  int idx=mainView->addWidget(widget);
  QAction* newAction=menuMain_View->addAction(title);
  mainViewMapper.setMapping(newAction,idx);
  connect(newAction,SIGNAL(triggered()),
	  &mainViewMapper,SLOT(map()));
}

void AGeoPlugin::handleNewEventLoaded(AEvent* event)
{
  emit eventLoaded(event);
}

Q_EXPORT_PLUGIN2(AGeoPlugin, AGeoPlugin)
