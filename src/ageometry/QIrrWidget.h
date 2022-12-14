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
#include <QGLWidget>
#include <QTime>
#include <QTimer>
#include <QFuture>
#include <QFutureWatcher>

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

#ifdef Q_WS_WIN
#include <windows.h>
#pragma comment(lib, "irrlicht.lib")
#endif //Q_WS_WIN


#include "AGeometryDefines.h"

class AGEOMETRY_EXPORT QIrrWidget : public QWidget,public IEventReceiver
{
public:
  QIrrWidget( QWidget *parent=0 );
  ~QIrrWidget();
  
  ISceneManager* getSceneManager();
  IVideoDriver* getVideoDriver();
  IGUIEnvironment* getGUIEnvironment();
  IFileSystem* getFileSystem();
  ITimer* getTimer();
  ICursorControl* getCursorControl();
  
  void setDirty(bool dirty);
  bool isDirty();
  
  void setDriverType( irr::video::E_DRIVER_TYPE driver );
  irr::video::E_DRIVER_TYPE driverType();
  
  QImage createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage,QRect baseRect=QRect() ,QRect overlayRect=QRect()); // Composites an overlayed image over another, with alpha channel
  
  /* STATIC HELPER FUNCTIONS */
  static QImage::Format Irr2Qt_ColorFormat(irr::video::ECOLOR_FORMAT); //Converts Irr colors format definitions to Qt color format definitions
  static EKEY_CODE Qt2Irr_KeyCode(int keycode);
  static int Irr2Qt_KeyCode(EKEY_CODE keycode);
  
  static u32 Qt2Irr_ButtonStates(Qt::MouseButtons buttons);
  static Qt::MouseButtons Irr2Qt_ButtonStates(u32 buttons);
					      
public slots:
  void toggleDisabled();
  void makeDirty();
  void forceUpdate();
  
signals:
  void cameraSwitched(int);
  void cameraSwitched(ICameraSceneNode *);
  void finishedLoading();
  
protected:
  /* Override these 3 functions in QIrrWidgets */
  virtual void load();
  virtual void execute();
  virtual bool OnEvent(const SEvent &event);
  
  /* Event */
  void paintEvent(QPaintEvent *event);
  void changeEvent(QEvent* event); //Will be used for diabled widget image caching
  void timerEvent(QTimerEvent*);
  
  void enterEvent(QEvent* event);
  void leaveEvent(QEvent* event);
  void showEvent(QShowEvent *event);
  void hideEvent(QHideEvent *event);
  
  virtual void mouseClickEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void keyPressEvent(QKeyEvent *event);
  virtual void keyReleaseEvent(QKeyEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);
  
  bool postEventFromUser(const SEvent& event);
  bool hasCameraMoved();
  
private:
  irr::video::E_DRIVER_TYPE _driverType;
  
  QWidget *p;    
  QPoint lastPressPos;
  int timerId;
  
  // Used for loading
  bool _ready;
  bool _loading;
  
  // Irrlicht things
  IrrlichtDevice* device;
  IVideoDriver* driver;
  ISceneManager* smgr;
  IFileSystem* fs;
  IGUIEnvironment* gui;
  ICursorControl* cursorcontrol;
  ITimer *timer;
  
  // Render checks
  bool _dirty;
  ICameraSceneNode *lastActiveCamera;
  vector3df lastCameraPosition,lastCameraTarget;
  float lastCameraFOV;
  
  // Screenshot
  video::ITexture *disabledRenderTexture;
  QPixmap ss;
  
  void internalLoad();
  
  void updateLastCamera();
  void updateScreenshot();
  
  Q_OBJECT
  
#ifdef Q_WS_WIN
  friend class QIrrWinWidgetPrivate;
#else
  friend class QIrrUnixWidgetPrivate;
#endif
};

#ifdef Q_WS_WIN
class QIrrWinWidgetPrivate : public QWidget
{
  
public:
  QIrrWinWidgetPrivate(QIrrWidget *parent);
  ~QIrrWinWidgetPrivate();
  
  
protected:
  virtual void resizeEvent( QResizeEvent *event );
  virtual void paintEvent( QPaintEvent *event );
  
  void initialize();
  
  QPaintEngine* paintEngine() const;
  
private:
  QIrrWidget* parent;
};
#else
class QIrrUnixWidgetPrivate : public QGLWidget
{    
public:
  QIrrUnixWidgetPrivate( QIrrWidget *parent=0 );
  ~QIrrUnixWidgetPrivate();
  
  void initializeGL();
  void paintGL();
  void resizeGL(int width,int height);
  
private:
  int timerId;
  
  QIrrWidget* parent;
};
#endif

#endif // QIRRWIDGET_H
