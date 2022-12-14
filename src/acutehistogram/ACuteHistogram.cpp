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

#include "ACuteHistogram.h"

#include <libamelia/AMELIA.h>

#include <ageometry/AGeoPlugin.h>

#include <QDebug>

ACuteHistogram::ACuteHistogram( QObject *parent )
  : QObject(parent),_event(0)
{
  _histWdg=new AHistogramWidget(100,0,2);
  _histWdg->show();
}

ACuteHistogram::~ACuteHistogram()
{ }

void ACuteHistogram::load()
{
  AGeoPlugin *geoplugin=(AGeoPlugin*)AMELIA::global->plugin("AGeometry");

  connect(geoplugin,SIGNAL(eventLoaded(AEvent *,AFilteredEvent *)),
	  this,SLOT(handleNewEventLoaded(AEvent *,AFilteredEvent *)));
}

void ACuteHistogram::handleNewEventLoaded(AEvent *event,AFilteredEvent *filteredevent)
{
  qDebug() << "New event" << filteredevent;
  if(_event)
    disconnect(_event,SIGNAL(filtersUpdated()),
	       this,SLOT(updateHistogram()));

  _histWdg->reset();
  
  _event=filteredevent;
  if(_event)
    {
      connect(_event,SIGNAL(filtersUpdated()),
	      this,SLOT(updateHistogram()));
      
      updateHistogram();
    }
}

void ACuteHistogram::updateHistogram()
{
  _histWdg->reset();
  qDebug() << "UPDATED?!?!";
  for(int i=0;i<_event->completeEvent()->Objects.size();i++)
    {
      AEventObject *obj=_event->completeEvent()->Objects[i];
      if(obj->type()!=AEventObject::eTrack) continue;
      
      if(_event->Objects.contains(obj))
	{
	  _histWdg->addData(Qt::blue,obj->pt());
	  qDebug() << "BLUE " << obj->pt();
	}
      else
	{
	  _histWdg->addData(Qt::gray,obj->pt());
	  qDebug() << "GRAY " << obj->pt();
	}
    }
}

Q_EXPORT_PLUGIN2(ACuteHistogram, ACuteHistogram)
