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



#ifndef ASELECTIONINFOSCENE_H
#define ASELECTIONINFOSCENE_H

#include <QList>

#include <QPushButton>

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>

#include "AGeometry.h"
#include "ATrackCombination.h"

class ASelectionInfoScene : public QGraphicsScene
{
  Q_OBJECT
    
public:
    ASelectionInfoScene(QObject* parent=0);
    ~ASelectionInfoScene();

public slots:
    void updateTrackInfo ( track* strack );
    void removeTrackInfo ( track* strack );
    void handleAddTrack();
    void handleCombTracks();
    void handleViewportChange(int from,int to);

    void displayMessage(QString message);
    void hideMessage();
    bool particleFilter(track* strack);

signals:
    void sig_addTrackToTable(track *strack);
    void sig_addComboToTable(ATrackCombination *combo);

    void combineButtonEnabled(bool status);
    void addButtonEnabled(bool status);

private:
    void init();

    track* selectedTrack;
    ATrackCombination *combo;

    QGraphicsTextItem *header;
    QGraphicsTextItem *nonSelectable;

    QGraphicsTextItem *name;
    QGraphicsTextItem *charge;
    QGraphicsTextItem *pt;
    QGraphicsTextItem *phi;
    QGraphicsTextItem *eta;
    QGraphicsTextItem *id;

    QGraphicsTextItem* dataComplete;
    QGraphicsTextItem* dataLabels;
    QGraphicsTextItem* dataDisplayed;

    QGraphicsTextItem* labelDisplayed;
    QGraphicsTextItem* labelTotal;

    QGraphicsProxyWidget *addTrack;
    QGraphicsProxyWidget *combTrack;
    QGraphicsTextItem *message;

    void refresh();
};
#endif
