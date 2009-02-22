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

#include "AWebPlugin.h"

#include <abase/ABase.h>

#include "ui_browser.h"


#include <QDebug>

AWebPlugin::AWebPlugin( QObject *parent )
        : QObject(parent)
{ }

AWebPlugin::~AWebPlugin() { }

void AWebPlugin::load()
{
  AMELIA *app=pluginBase();
  ABase *base=(ABase *)app->plugin("ABase");

  Ui::BrowserWindow browserUI;

  QMainWindow *newswdg=new QMainWindow();
  QMainWindow *wikiwdg=new QMainWindow();
  
  browserUI.setupUi(newswdg);
  setURL(newswdg,"http://amelia.sourceforge.net/");
  addButton(newswdg,"Menu","");

  browserUI.setupUi(wikiwdg);
  setURL(wikiwdg,"http://amelia.sourceforge.net/usingamelia.html");
  addButton(wikiwdg,"Menu","");

  base->addLevel("wiki","Default",wikiwdg,"How to use AMELIA");
  base->addLevel("news","Default",newswdg,"The AMELIA Portal!");

  connect(&abaseMapper,SIGNAL(mapped(const QString&)),
	  base,SLOT(changeToMonitor(const QString&)));
	  
}

void AWebPlugin::setURL(QWidget *browserwdg,QString url)
{
  QWebView* MainWebView=browserwdg->findChild<QWebView*>("MainWebView");
  MainWebView->setUrl(url);
}

void AWebPlugin::addButton(QWidget *browserwdg,QString text,QString link)
{
  ADropDownMenu *menu=browserwdg->findChild<ADropDownMenu*>("DropDownMenu");
  
  QAction *action=new QAction(text,menu);
  menu->addAction(action);
  
  abaseMapper.setMapping(action,link);
  connect(action,SIGNAL(triggered()),
	  &abaseMapper,SLOT(map()));
}

Q_EXPORT_PLUGIN2(AWebPlugin, AWebPlugin)
