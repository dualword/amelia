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


#include "QIrrWidget.h"

#include <QAction>
#include <QApplication>
#include <QGridLayout>
#include <QtConcurrentRun>

#ifndef Q_WS_WIN
namespace irr
{
class CTimer : public ITimer
{
	virtual u32 getRealTime() const;
    virtual u32 getTime() const;
	virtual void setTime(u32 time);
	virtual void stop();
	virtual void start();
	virtual void setSpeed(f32 speed = 1.0f);
	virtual bool isStopped() const;
	virtual void tick();
	virtual f32 getSpeed() const;
};

#ifdef Q_WS_MAC
  class CIrrDeviceMacOSX;
#endif
  
namespace video
{	
#ifdef Q_WS_MAC
	IVideoDriver* createOpenGLDriver(const SIrrlichtCreationParameters& param, irr::io::IFileSystem* io, CIrrDeviceMacOSX *device);
#else
	IVideoDriver* createOpenGLDriver(const SIrrlichtCreationParameters& params, io::IFileSystem* io);
#endif
}
  
namespace scene
{
    ISceneManager* createSceneManager(video::IVideoDriver* driver,
										io::IFileSystem* fs, gui::ICursorControl* cc, gui::IGUIEnvironment *gui);
}
namespace io
{
    IFileSystem* createFileSystem();
}
  
namespace gui
{
    IGUIEnvironment* createGUIEnvironment(irr::io::IFileSystem* fs,
										video::IVideoDriver* Driver, IOSOperator* op);
}
  
namespace os
{
    class Timer
    {
    public:
	static void initTimer();
	};
}
} // end namespace irr
class CCursorControl : public gui::ICursorControl
{
public:
  CCursorControl(QIrrWidget *_parent):parent(_parent),IsVisible(true) { }
  
  //! Changes the visible state of the mouse cursor.
  virtual void setVisible(bool visible)
  {
    if(visible)
      parent->setCursor(Qt::ArrowCursor);
    else
      parent->setCursor(QCursor( Qt::BlankCursor ));
    
    IsVisible=false;  }
  
  //! Returns if the cursor is currently visible.
  virtual bool isVisible() const
  {
    return IsVisible;
  }
  
  //! Sets the new position of the cursor.
  virtual void setPosition(const core::position2d<f32> &pos)
  {
    setPosition(pos.X,pos.Y);
  }
  
  //! Sets the new position of the cursor.
  virtual void setPosition(f32 x, f32 y)
  {
    QPoint pos(x*parent->width(),y*parent->height());
    QCursor::setPos(parent->mapToGlobal(pos));
  }
  
  //! Sets the new position of the cursor.
  virtual void setPosition(const core::position2d<s32> &pos)
  {
    setPosition(pos.X,pos.Y);
  }
  
  //! Sets the new position of the cursor.
  virtual void setPosition(s32 x, s32 y)
  {
    QCursor::setPos(parent->mapToGlobal(QPoint(x,y)));
  }
  
  //! Returns the current position of the mouse cursor.
  virtual core::position2d<s32> getPosition()
  {
    QPoint pos=parent->mapFromGlobal(QCursor::pos());
    return core::position2d<s32>(pos.x(),pos.y());
  }
  
  //! Returns the current position of the mouse cursor.
  virtual core::position2d<f32> getRelativePosition()
  {
    QPoint pos=parent->mapFromGlobal(QCursor::pos());
    QPointF posF(((float)pos.x())/parent->width(),
		 ((float)pos.y())/parent->height());
    return core::position2d<f32>(posF.x(),posF.y());
  }
  
  virtual void setReferenceRect(core::rect<s32>* rect=0)
  {

  }
  
private:
  bool IsVisible;
  QIrrWidget *parent;
};
#endif


QIrrWidget::QIrrWidget( QWidget *parent )
  : QWidget(parent),driver(0),smgr(0),gui(0),
    _dirty(false),disabledRenderTexture(0),
    _ready(false),_loading(false),timerId(-1)
{
  // Default to Open GL
#ifdef Q_WS_WIN
  _driverType = irr::video::EDT_DIRECT3D9;
#else
  _driverType = irr::video::EDT_OPENGL;
#endif
  
  QGridLayout *layout=new QGridLayout(this);
  layout->setContentsMargins(0,0,0,0);
  
#ifdef Q_WS_WIN
  p=new QIrrWinWidgetPrivate(this);
#else
  p=new QIrrUnixWidgetPrivate(this);
#endif
  
  layout->addWidget(p);
  
  setMouseTracking(true);

  ss=QPixmap(":/initial.png");
}

QIrrWidget::~QIrrWidget()
{ }


video::IVideoDriver* QIrrWidget::getVideoDriver()
{
  return driver;
}

ISceneManager* QIrrWidget::getSceneManager()
{
  return smgr;
}

IGUIEnvironment* QIrrWidget::getGUIEnvironment()
{
  return gui;
}

IFileSystem* QIrrWidget::getFileSystem()
{
  return fs;
}

ITimer* QIrrWidget::getTimer()
{
  return timer;
}

ICursorControl* QIrrWidget::getCursorControl()
{
  return cursorcontrol;
}

irr::video::E_DRIVER_TYPE QIrrWidget::driverType()
{
  return _driverType;
}

void QIrrWidget::makeDirty()
{
  _dirty=true;
}

void QIrrWidget::setDirty(bool dirty)
{
  _dirty=dirty;
}

bool QIrrWidget::isDirty()
{
  return _dirty;
}

void QIrrWidget::toggleDisabled()
{
  setEnabled(!isEnabled());
}

void QIrrWidget::updateScreenshot()
{
  //Attempt at using RTT for display. This fixes the problem with the initial render
  //However right now it does not work unless the screenshot is updated continiously
  //Performing the update only from the change event call causes the same picture to
  //be captured always...
  /*if (getVideoDriver()->queryFeature(video::EVDF_RENDER_TO_TARGET))
    { //We can render to target.. Use a RTT
      irr::video::SColor color (255,0,0,0);
      
      if (disabledRenderTexture==0) //Try to create an render texture if one does not exist...
	{
	  QSize size=p->size();
	  //int w=256;
	  //int h=256;
	  //Note: This might cause problems, since the Irrlicht docs suggest
	  // that the RTT size is a square with lengths of powers of 2
	  int w=size.width();
	  int h=size.height();
	  disabledRenderTexture = getVideoDriver()->addRenderTargetTexture(core::dimension2d<s32>(w,h));
	  disabledRenderTexture->grab();
	}
      
      //Render
      driver->setRenderTarget(disabledRenderTexture, true, true, color);
      smgr->drawAll();
      driver->setRenderTarget(0);
      
      //Convert RTT to QImage
      uchar* tmpdata=(uchar*)disabledRenderTexture->lock (true);
      dimension2d<s32> size=disabledRenderTexture->getSize();
      QImage image(tmpdata,size.Width,size.Height,QIrrWidget::Irr2Qt_ColorFormat(disabledRenderTexture->getColorFormat()));
      disabledRenderTexture->unlock();
      
      ss=QPixmap::fromImage(image);
    }
    else*/
  
  // For some reason, this is visible at the first screenshot and invisible at the second one
  if(!p->isVisible())
    ss=QPixmap::grabWindow(p->winId());  
}

void QIrrWidget::forceUpdate()
{
  QTimerEvent timer(timerId);
  makeDirty();
  timerEvent(&timer);
}

void QIrrWidget::paintEvent(QPaintEvent *event)
{
  if(isEnabled())
    QWidget::paintEvent(event);
  else
    {
      QPainter painter(this);
      painter.drawPixmap(0,0,ss);
    }
}
  
void QIrrWidget::changeEvent(QEvent *event)
{
  if(event->type()==QEvent::EnabledChange)
    {
      setUpdatesEnabled(false);
      if(isEnabled())
	{
	  p->show();
	}
      else
	{
	  if(driver)
	    {
	      updateScreenshot();
	    }
	  p->hide();
	}
      setUpdatesEnabled(true);
    }

  QWidget::changeEvent(event);
}

void QIrrWidget::resizeEvent( QResizeEvent* event )
{
  if ( driver != 0 )
    {
      irr::core::dimension2d<int> size;
      size.Width = event->size().width();
      size.Height = event->size().height();

      driver->OnResize( size );
    }
  QWidget::resizeEvent(event);
}


void QIrrWidget::execute()
{ }

void QIrrWidget::updateLastCamera()
{
  if(!_ready) return;

  ICameraSceneNode *activeCam=smgr->getActiveCamera();

  lastActiveCamera=smgr->getActiveCamera();
  lastCameraPosition=lastActiveCamera->getPosition();
  lastCameraTarget=lastActiveCamera->getTarget();
}

bool QIrrWidget::OnEvent(const SEvent &event)
{
  return false;
}

void QIrrWidget::load() { }

void QIrrWidget::internalLoad()
{
  _loading=true;
  load();
  _ready=true;
  _loading=false;
}

QImage QIrrWidget::createImageWithOverlay(const QImage& baseImage, const QImage& overlayImage, QRect baseRect, QRect overlayRect)
{
    if (baseRect.isNull()) baseRect=baseImage.rect();
    if (overlayRect.isNull()) overlayRect=overlayImage.rect();

    QSize newSize;
    if (baseRect.height()>overlayRect.height())
    {
        newSize=baseImage.size();
    }
    else
    {
        newSize=overlayImage.size();
    }

    QImage imageWithOverlay = QImage(newSize, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&imageWithOverlay);


    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(baseRect, baseImage);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(overlayRect, overlayImage);

    painter.end();

    return imageWithOverlay;
}

QImage::Format QIrrWidget::Irr2Qt_ColorFormat(irr::video::ECOLOR_FORMAT format)
{
    switch (format)
    {
    case ECF_A1R5G5B5:
        return QImage::Format_RGB555;
        break;
    case ECF_R5G6B5:
        return QImage::Format_RGB16;
        break;
    case ECF_R8G8B8:
        return QImage::Format_RGB888;
        break;
    case ECF_A8R8G8B8:
        return QImage::Format_ARGB32;
        break;
    }
    return QImage::Format_Invalid;
}

EKEY_CODE QIrrWidget::Qt2Irr_KeyCode(int keycode)
{
  //Ok, seriously. Someone needs to come up with a standard keycode mapping....

  switch(keycode)
    {
    case Qt::Key_Escape:
      return KEY_ESCAPE;
    case Qt::Key_Tab:
      return KEY_TAB;
    case Qt::Key_Cancel:
      return KEY_CANCEL;
    case Qt::Key_Backspace:
      return KEY_BACK;
    case Qt::Key_Clear:
      return KEY_CLEAR;
    case Qt::Key_Return:
      return KEY_RETURN;
    case Qt::Key_Shift:
      return KEY_SHIFT;
    case Qt::Key_Control:
      return KEY_CONTROL;
    case Qt::Key_Menu:
      return KEY_MENU;
    case Qt::Key_Pause:
      return KEY_PAUSE;
    case Qt::Key_CapsLock:
      return KEY_CAPITAL;
    case Qt::Key_Hangul:
      return KEY_HANGUL;
    case Qt::Key_Kanji:
      return KEY_KANJI;
    case Qt::Key_Mode_switch:
      return KEY_MODECHANGE;
    case Qt::Key_Space:
      return KEY_SPACE;
    case Qt::Key_Back:
    case Qt::Key_PageUp:
      return KEY_PRIOR;
    case Qt::Key_Forward:
    case Qt::Key_PageDown:
      return KEY_NEXT;
    case Qt::Key_End:
      return KEY_END;
    case Qt::Key_Home:
      return KEY_HOME;
    case Qt::Key_Left:
      return KEY_LEFT;
    case Qt::Key_Up:
      return KEY_UP;
    case Qt::Key_Right:
      return KEY_RIGHT;
    case Qt::Key_Down:
      return KEY_DOWN;
    case Qt::Key_Select:
      return KEY_SELECT;
    case Qt::Key_Execute:
      return KEY_EXECUT;
    case Qt::Key_Print:
      return KEY_PRINT;
    case Qt::Key_Insert:
      return KEY_INSERT;
    case Qt::Key_Delete:
      return KEY_DELETE;
    case Qt::Key_Help:
      return KEY_HELP;
    case Qt::Key_Meta:
      return KEY_LWIN; //There is no right/left Meta keys in Qt
    case Qt::Key_Sleep:
      return KEY_SLEEP;
    case Qt::Key_Comma:
      return KEY_COMMA;
    case Qt::Key_Asterisk:
      return KEY_MULTIPLY;
    case Qt::Key_Plus:
      return KEY_ADD;
    case Qt::Key_Slash:
      return KEY_DIVIDE;
    case Qt::Key_Minus:
      return KEY_MINUS;
    case Qt::Key_Period:
      return KEY_PERIOD;
    case Qt::Key_multiply:
      return KEY_MULTIPLY;
    case Qt::Key_ScrollLock:
      return KEY_SCROLL;
    case Qt::Key_NumLock:
      return KEY_NUMLOCK;
    case Qt::Key_Play:
      return KEY_PLAY;
    case Qt::Key_Zoom:
      return KEY_ZOOM;

    default:
      //In Qt 4.4 and Irrlicht 1.5 (svn), the alphanumeric mapping are the same
      // Numbers 0x30 - 0x39
      // Letters 0x41 - 0x5A
      if(keycode > 0x30 && keycode < 0x39)
	return (irr::EKEY_CODE)keycode;
      if(keycode > 0x41 && keycode < 0x5A)
	return (irr::EKEY_CODE)keycode;

      //F# keys
      // Qt 4.4: 0x01000030 +35
      // Irrlicht: 0x70 +24
      if(keycode > 0x01000030 && keycode < 0x01000030+24)
	return (irr::EKEY_CODE)(keycode-0xFFFFC0);
      break;
    }
  return (irr::EKEY_CODE)NULL;
}

int QIrrWidget::Irr2Qt_KeyCode(EKEY_CODE keycode)
{
  //See comment in previous post about mappings....

  switch(keycode)
    {
    case KEY_ESCAPE:
      return Qt::Key_Escape;
    case KEY_TAB:
      return Qt::Key_Tab;
    case KEY_CANCEL:
      return Qt::Key_Cancel;
    case KEY_BACK:
      return Qt::Key_Backspace;
    case KEY_CLEAR:
      return Qt::Key_Clear;
    case KEY_RETURN:
      return Qt::Key_Return;
    case KEY_SHIFT:
      return Qt::Key_Shift;
    case KEY_CONTROL:
      return Qt::Key_Control;
    case KEY_MENU:
      return Qt::Key_Menu;
    case KEY_PAUSE:
      return Qt::Key_Pause;
    case KEY_CAPITAL:
      return Qt::Key_CapsLock;
    case KEY_HANGUL:
      return Qt::Key_Hangul;
    case KEY_KANJI:
      return Qt::Key_Kanji;
    case KEY_MODECHANGE:
      return Qt::Key_Mode_switch;
    case KEY_SPACE:
      return Qt::Key_Space;
    case KEY_PRIOR:
      return Qt::Key_Back;
    case KEY_NEXT:
      return Qt::Key_Forward;
    case KEY_END:
      return Qt::Key_End;
    case KEY_HOME:
      return Qt::Key_Home;
    case KEY_LEFT:
      return Qt::Key_Left;
    case KEY_UP:
      return Qt::Key_Up;
    case KEY_RIGHT:
      return Qt::Key_Right;
    case KEY_DOWN:
      return Qt::Key_Down;
    case KEY_SELECT:
      return Qt::Key_Select;
    case KEY_EXECUT:
      return Qt::Key_Execute;
    case KEY_PRINT:
      return Qt::Key_Print;
    case KEY_INSERT:
      return Qt::Key_Insert;
    case KEY_DELETE:
      return Qt::Key_Delete;
    case KEY_HELP:
      return Qt::Key_Help;
    case KEY_LWIN: //There is no right/left Meta keys in Qt
      return Qt::Key_Meta;
    case KEY_SLEEP:
      return Qt::Key_Sleep;
    case KEY_COMMA:
      return Qt::Key_Comma;
    case KEY_MULTIPLY:
      return Qt::Key_Asterisk;
    case KEY_ADD:
      return Qt::Key_Plus;
    case KEY_DIVIDE:
      return Qt::Key_Slash;
    case KEY_MINUS:
      return Qt::Key_Minus;
    case KEY_PERIOD:
      return Qt::Key_Period;
    case KEY_SCROLL:
      return Qt::Key_ScrollLock;
    case KEY_NUMLOCK:
      return Qt::Key_NumLock;
    case KEY_PLAY:
      return Qt::Key_Play;
    case KEY_ZOOM:
      return Qt::Key_Zoom;

    default:
      //In Qt 4.4 and Irrlicht 1.5 (svn), the alphanumeric mapping are the same (thank god!)
      // Numbers 0x30 - 0x39
      // Letters 0x41 - 0x5A
      if(keycode > 0x30 && keycode < 0x39)
	return (int)keycode;
      if(keycode > 0x41 && keycode < 0x5A)
	return (int)keycode;

      //F# keys
      // Qt 4.4: 0x01000030 +35
      // Irrlicht: 0x70 +24
      if(keycode > 0x70 && keycode < 0x70+24)
	return ((int)keycode+0xFFFFC0);
      break;
    }
  return 0;
}

void QIrrWidget::timerEvent(QTimerEvent *event)
{
  if(!isEnabled()) return;

  if(!_ready && !_loading)
    internalLoad();

  timer->tick();

  if(_ready)
    execute();

  if(hasCameraMoved() || isDirty())
    {
      p->repaint();
      updateLastCamera();
      setDirty(false);
    }
  else
    {
      smgr->getRootSceneNode()->OnAnimate(timer->getTime());
    }
}

void QIrrWidget::showEvent(QShowEvent *event)
{
  if(timerId==-1)
    timerId=startTimer(40);
}

void QIrrWidget::hideEvent(QHideEvent *event)
{
  killTimer(timerId);
  timerId=-1;
}

void QIrrWidget::enterEvent(QEvent* event)
{
  if(!_ready) return;

  setFocus();
  grabKeyboard();

  QWidget::enterEvent(event);
}

void QIrrWidget::leaveEvent(QEvent* event)
{
  if(!_ready) return;

  clearFocus();
  releaseKeyboard();

  QWidget::leaveEvent(event);
}

void QIrrWidget::keyPressEvent(QKeyEvent *event)
{
  SEvent e;
  e.EventType=irr::EET_KEY_INPUT_EVENT;
  e.KeyInput.Char=event->text()[0].toAscii();
  e.KeyInput.Control=event->modifiers() & Qt::ControlModifier;
  e.KeyInput.Key=QIrrWidget::Qt2Irr_KeyCode(event->key());
  e.KeyInput.PressedDown=true;
  e.KeyInput.Shift=event->modifiers() & Qt::ShiftModifier;
  if(postEventFromUser(e))
    event->accept();

  QWidget::keyPressEvent(event);
}

void QIrrWidget::keyReleaseEvent(QKeyEvent *event)
{
  SEvent e;
  e.EventType=irr::EET_KEY_INPUT_EVENT;
  e.KeyInput.Char=event->text()[0].toAscii();
  e.KeyInput.Control=event->modifiers() & Qt::ControlModifier;
  e.KeyInput.Key=QIrrWidget::Qt2Irr_KeyCode(event->key());
  e.KeyInput.PressedDown=false;
  e.KeyInput.Shift=event->modifiers() & Qt::ShiftModifier;
  if(postEventFromUser(e))
    event->accept();

  QWidget::keyReleaseEvent(event);
}

void QIrrWidget::mouseClickEvent(QMouseEvent *event)
{ }

void QIrrWidget::mouseMoveEvent(QMouseEvent *event)
{
  SEvent e;
  e.EventType = irr::EET_MOUSE_INPUT_EVENT;
  e.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
  e.MouseInput.X = event->x();
  e.MouseInput.Y = event->y();
  if(postEventFromUser(e))
    event->accept();

  QWidget::mouseMoveEvent(event);
}

void QIrrWidget::mousePressEvent(QMouseEvent *event)
{
  SEvent e;
  e.EventType = irr::EET_MOUSE_INPUT_EVENT;
  switch(event->button())
    {
    case Qt::LeftButton:
      e.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
      break;
    case Qt::MidButton:
      e.MouseInput.Event = irr::EMIE_MMOUSE_PRESSED_DOWN;
      break;
    case Qt::RightButton:
      e.MouseInput.Event = irr::EMIE_RMOUSE_PRESSED_DOWN;
      break;
    default:
      e.MouseInput.Event = irr::EMIE_COUNT;
      break;
    }
  
  e.MouseInput.X = event->x();
  e.MouseInput.Y = event->y();
  
  lastPressPos=event->pos();

  if(postEventFromUser(e))
    event->accept();

  QWidget::mousePressEvent(event);
}

void QIrrWidget::mouseReleaseEvent(QMouseEvent *event)
{
  SEvent e;
  e.EventType = irr::EET_MOUSE_INPUT_EVENT;
  switch(event->button())
    {
    case Qt::LeftButton:
      e.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
      break;
    case Qt::MidButton:
      e.MouseInput.Event = irr::EMIE_MMOUSE_LEFT_UP;
      break;
    case Qt::RightButton:
      e.MouseInput.Event = irr::EMIE_RMOUSE_LEFT_UP;
      break;
    default:
      e.MouseInput.Event = irr::EMIE_COUNT;
      break;
    }

  e.MouseInput.X = event->x();
  e.MouseInput.Y = event->y();
  
  if(postEventFromUser(e))
    event->accept();
  
  if((event->pos()-lastPressPos).manhattanLength()<1)
    mouseClickEvent(event);

  QWidget::mouseReleaseEvent(event);
}

void QIrrWidget::wheelEvent(QWheelEvent *event)
{
  SEvent e;
  e.EventType = irr::EET_MOUSE_INPUT_EVENT;
  e.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
  e.MouseInput.X = event->x();
  e.MouseInput.Y = event->y();
  e.MouseInput.Wheel = event->delta()/qAbs(event->delta()); //Irrlicht uses values and 1 to -1. So let's just do that.
  if(postEventFromUser(e))
    event->accept();

  QWidget::wheelEvent(event);
}

bool QIrrWidget::postEventFromUser(const SEvent& event)
{
  bool absorbed = false;
  
  if (!absorbed && getGUIEnvironment())
    absorbed|=getGUIEnvironment()->postEventFromUser(event);


  if(!absorbed && getSceneManager())
    absorbed|=getSceneManager()->postEventFromUser(event);

  return absorbed;
}

bool QIrrWidget::hasCameraMoved()
{
  if(!_ready) return true;

  ICameraSceneNode* activeCam=smgr->getActiveCamera();
  bool cameraChanged=(lastActiveCamera!=activeCam);
  bool cameraMoved=(lastCameraPosition!=activeCam->getPosition());
  bool cameraTargetMoved=(lastCameraTarget!=activeCam->getTarget());

  return cameraChanged || cameraMoved || cameraTargetMoved;
}

#ifdef Q_WS_WIN
QIrrWinWidgetPrivate::QIrrWinWidgetPrivate( QIrrWidget *parent )
  : QWidget(parent)
{
  // Irrlicht will clear the canvas for us, so tell Qt not to do it
  setAttribute( Qt::WA_OpaquePaintEvent );
  //Tell Qt we will use something else for painting
  setAttribute(Qt::WA_PaintOnScreen);
  setAttribute(Qt::WA_NoBackground);
  setAttribute(Qt::WA_NoSystemBackground);
  
  setMouseTracking(true);
  
  setFocusPolicy(Qt::StrongFocus);
  setAutoFillBackground(false);

  this->parent=parent;
}

QIrrWinWidgetPrivate::~QIrrWinWidgetPrivate()
{
  killTimer(timerId);
}

void QIrrWinWidgetPrivate::initialize()
{
  // Don't initialize more than once!
  if ( parent->driver != 0 ) return;
  irr::SIrrlichtCreationParameters params;
  
  params.DriverType = parent->driverType();
  params.WindowId = (void*)winId();
  params.WindowSize.Width = (width()>0)?width():1;
  params.WindowSize.Height = (height()>0)?height():1;
  params.AntiAlias = true;
  params.IgnoreInput = true;

  device=createDeviceEx(params);
  
  parent->driver=device->getVideoDriver();
  
  parent->gui=device->getGUIEnvironment();
  parent->fs=device->getFileSystem();

  parent->cursorcontrol = device->getCursorControl();
  parent->smgr=device->getSceneManager();

  parent->internalLoad();
  
  parent->update();

  timerId=startTimer(30);

}

void QIrrWinWidgetPrivate::timerEvent(QTimerEvent* event)
{
  if(isVisible() && isEnabled())
    update();
}
  
void QIrrWinWidgetPrivate::paintEvent( QPaintEvent* event )
{
	initialize();
  if (parent->driver)
    {
      device->run();

	  if(parent->_ready)
        parent->execute();

      irr::video::SColor color (0,0,0,0);
      
      parent->driver->beginScene(true,true,color);
      
      parent->smgr->drawAll();
      parent->gui->drawAll();
      
      parent->driver->endScene();
    }
}

QPaintEngine* QIrrWinWidgetPrivate::paintEngine() const
{
	return 0;
}
#else
QIrrUnixWidgetPrivate::QIrrUnixWidgetPrivate( QIrrWidget *parent )
  : QGLWidget(QGLFormat(QGL::SampleBuffers),parent),timerId(-1)
{
  setMouseTracking(true);
  
  setFocusPolicy(Qt::StrongFocus);
  setAutoFillBackground(false);
  setAutoBufferSwap(false);

  this->parent=parent;
}

QIrrUnixWidgetPrivate::~QIrrUnixWidgetPrivate()
{
  killTimer(timerId);

  //Must drop stuff I created.
  parent->driver->drop();
  parent->smgr->drop();
  parent->gui->drop();
  parent->timer->drop();
}

void QIrrUnixWidgetPrivate::initializeGL()
{
  // Don't initialize more than once!
  if ( parent->driver != 0 ) return;
  irr::SIrrlichtCreationParameters params;
  
  params.DriverType = video::EDT_OPENGL;
  params.WindowId = (void*)winId();
  params.WindowSize.Width = (width()>0)?width():1;
  params.WindowSize.Height = (height()>0)?height():1;
  params.AntiAlias = true;
  params.IgnoreInput = true;

  parent->fs = io::createFileSystem();
  
#ifdef Q_WS_MAC
  parent->driver=createOpenGLDriver(params,parent->fs,0);
#else
  parent->driver=createOpenGLDriver(params,parent->fs);
#endif
  
  parent->gui=createGUIEnvironment(parent->fs,parent->driver,0);

  parent->cursorcontrol = new CCursorControl(parent);
  parent->smgr=scene::createSceneManager(parent->driver,parent->fs,parent->cursorcontrol,parent->gui);

  os::Timer::initTimer();  
  parent->timer = new CTimer();
}

void QIrrUnixWidgetPrivate::paintGL()
{
  if (parent->driver && (parent->_ready || parent->_loading))
    {
      irr::video::SColor color (255,0,0,0);

      parent->driver->beginScene(true,true,color);

      parent->smgr->drawAll();
      parent->gui->drawAll();
      parent->driver->endScene();

      /*static int c=0;
      QPixmap ss=QPixmap::grabWindow(winId());  
      ss.save("images/"+QString::number(c)+".jpg");
      c++;*/
    }
}

#endif
