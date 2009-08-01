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



#ifndef AXMLEVENT_H
#define AXMLEVENT_H

#include <QDomNode>
#include <QDomDocument>
#include <QFileInfo>

#include "AEvent.h"
#include <irrlicht.h>
#include <vector>



class AEVENTMANAGER_EXPORT AXmlEvent : public AEvent
{
  Q_OBJECT
 public:
  QString name;
  bool isLoaded;

  AXmlEvent(QString file);
  virtual ~AXmlEvent();

  static AXmlEvent* CachedEvent(QString file);

  void GetEventFromFile ( QString filename );
  void HideAllTracks();
  void LoadEvent();

  QList<bool> P_checkbox_states;
  void setCurrentJetModel(QString jetType);
  void setCurrentTrackModel(QString trackType);
  void setCurrentMisEtModel(QString metType);

  void setPackage(AEventPackage *pkg);

 signals:
  // Emitted whenever the displayed event changes shape (ei: during pT cutoff)
  void eventChanged();

 private:
  static QMap<QString,AXmlEvent*> cache;

  QList<int> getDataInt ( QDomNode xml );
  QList<float> getDataFloat ( QDomNode xml );
  void GetSTracksFromDOM ( QDomDocument doc);
  void GetRTracksFromDOM ( QDomDocument doc);
  void GetJetsFromDOM ( QDomDocument doc);
  void GetMisETFromDOM ( QDomDocument doc);
  void GetShowersFromDOM ( QDomDocument doc, QString calo );
  void GetFCALShowersFromDOM ( QDomDocument doc );

  QString filename;
  QString location;
};

#endif //AXMLEVENT_H
