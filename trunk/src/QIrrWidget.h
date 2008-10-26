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




#ifndef QIRRWIDGET_H
#define QIRRWIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include <QComboBox>
#include <QPainter>
#include <QDebug>
#include <QLabel>

#ifdef Q_WS_WIN
#include <windows.h>
#endif //Q_WS_WIN

#include <irrlicht.h>
#include <ITimer.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <sstream>


using namespace irr;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::core;
using namespace io;
using namespace std;
using namespace gui;

#pragma comment(lib, "irrlicht.lib")


class QIrrWidget : public QWidget,public IEventReceiver
{
    Q_OBJECT

public:
    QIrrWidget( QWidget *parent=0 );
    ~QIrrWidget();

    irr::IrrlichtDevice* GetDevice();
    ISceneManager* GetSceneManager();
    video::IVideoDriver* GetDriver();

    void setDriverType( irr::video::E_DRIVER_TYPE driver );
    irr::video::E_DRIVER_TYPE driverType();

    QImage screenshot(); //Returns the screenshot of the current rendered scene
    QImage createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage,QRect baseRect=QRect() ,QRect overlayRect=QRect()); // Composites an overlayed image over another, with alpha channel

    /* STATIC HELPER FUNCTIONS */
    static QImage::Format Irr2Qt_ColorFormat(irr::video::ECOLOR_FORMAT); //Converts Irr colors format definitions to Qt color format definitions
    static EKEY_CODE Qt2Irr_KeyCode(int keycode);
    static int Irr2Qt_KeyCode(EKEY_CODE keycode);

public slots:
  void toggleDisabled();

protected:
    irr::IrrlichtDevice* Device;

    /* Override these 3 functions in QIrrWidgets */
    virtual void load();
    virtual void execute();
    virtual bool OnEvent(const SEvent &event);

    /* Event */
    void changeEvent(QEvent* event); //Will be used for diabled widget image caching

private:
    irr::video::E_DRIVER_TYPE _driverType;

    QLabel *label;
  class QIrrWidgetPrivate *p;
  friend class QIrrWidgetPrivate;
};

class QIrrWidgetPrivate : public QWidget
{
 public:
  QIrrWidgetPrivate(QIrrWidget *parent);
  ~QIrrWidgetPrivate();

  IrrlichtDevice* initialize(irr::video::E_DRIVER_TYPE _driverType);

  virtual QPaintEngine * paintEngine () const;

protected:
  virtual void paintEvent( QPaintEvent* event );
  virtual void timerEvent( QTimerEvent* event );
  virtual void resizeEvent( QResizeEvent* event );

  // Enter/leave events are used for grabbing keyboard focus on hover
  void enterEvent(QEvent* event);
  void leaveEvent(QEvent* event);
  
  //Used for converting Qt input events into Irrlich events
  void mouseMoveEvent(QMouseEvent *event);  
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  
 private:
  IrrlichtDevice* Device;
  int timerId;
};

#endif // QIRRWIDGET_H