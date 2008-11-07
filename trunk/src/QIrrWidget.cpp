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

QIrrWidget::QIrrWidget( QWidget *parent )
        : QWidget(parent)
{
    // Wait for the init() call
    Device = 0;

    // Default to Open GL
#ifdef Q_WS_WIN
	_driverType = irr::video::EDT_DIRECT3D9;
#else
    _driverType = irr::video::EDT_OPENGL;
#endif

    QGridLayout *layout=new QGridLayout(this);
    p=new QIrrWidgetPrivate(this);
    layout->addWidget(p);

    label=new QLabel(this);
    layout->addWidget(label);
    label->hide();
}

QIrrWidget::~QIrrWidget()
{
}


irr::IrrlichtDevice* QIrrWidget::GetDevice()
{
    return Device;
}

video::IVideoDriver* QIrrWidget::GetDriver()
{
    return Device->getVideoDriver();
}

ISceneManager* QIrrWidget::GetSceneManager()
{
    return Device->getSceneManager();
}

irr::video::E_DRIVER_TYPE QIrrWidget::driverType()
{
  return _driverType;
}

void QIrrWidget::toggleDisabled()
{
  setEnabled(!isEnabled());
}

void QIrrWidget::changeEvent(QEvent *event)
{
  if(event->type()==QEvent::EnabledChange)
    {
      setUpdatesEnabled(false);
      if(isEnabled())
	{
	  label->hide();
	  label->setPixmap(0);
	  p->show();
	}
      else
	{
	  QPixmap ss;
	  if(Device)
	    ss=QPixmap::grabWindow(p->winId());

	  p->hide();

	  label->setPixmap(ss);
	  label->show();
	}
      setUpdatesEnabled(true);
    }

  QWidget::changeEvent(event);
}

void QIrrWidget::execute() { }

bool QIrrWidget::OnEvent(const SEvent &event)
{
  return false;
}

void QIrrWidget::load() { }

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
      return KEY_PRIOR;
    case Qt::Key_Forward:
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

void QIrrWidgetPrivate::enterEvent(QEvent* event)
{
  setFocus();
  grabKeyboard();

  QWidget::enterEvent(event);
}

void QIrrWidgetPrivate::leaveEvent(QEvent* event)
{
  clearFocus();
  releaseKeyboard();

  QWidget::leaveEvent(event);
}

void QIrrWidgetPrivate::keyPressEvent(QKeyEvent *event)
{
  if(!Device) return;
  SEvent e;
  e.EventType=irr::EET_KEY_INPUT_EVENT;
  e.KeyInput.Char=event->text()[0].toAscii();
  e.KeyInput.Control=event->modifiers() & Qt::ControlModifier;
  e.KeyInput.Key=QIrrWidget::Qt2Irr_KeyCode(event->key());
  e.KeyInput.PressedDown=true;
  e.KeyInput.Shift=event->modifiers() & Qt::ShiftModifier;
  if(Device->postEventFromUser(e))
    event->accept();

  QWidget::keyPressEvent(event);
}

void QIrrWidgetPrivate::keyReleaseEvent(QKeyEvent *event)
{
  if(!Device) return;
  SEvent e;
  e.EventType=irr::EET_KEY_INPUT_EVENT;
  e.KeyInput.Char=event->text()[0].toAscii();
  e.KeyInput.Control=event->modifiers() & Qt::ControlModifier;
  e.KeyInput.Key=QIrrWidget::Qt2Irr_KeyCode(event->key());
  e.KeyInput.PressedDown=false;
  e.KeyInput.Shift=event->modifiers() & Qt::ShiftModifier;
  if(Device->postEventFromUser(e))
    event->accept();

  QWidget::keyReleaseEvent(event);
}

void QIrrWidgetPrivate::mouseMoveEvent(QMouseEvent *event)
{
  if(!Device) return;
  SEvent e;
  e.EventType = irr::EET_MOUSE_INPUT_EVENT;
  e.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
  e.MouseInput.X = event->x();
  e.MouseInput.Y = event->y();
  if(Device->postEventFromUser(e))
    event->accept();

  QWidget::mouseMoveEvent(event);
}

void QIrrWidgetPrivate::mousePressEvent(QMouseEvent *event)
{
  if(!Device) return;
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

  if(Device->postEventFromUser(e))
    event->accept();

  QWidget::mousePressEvent(event);
}

void QIrrWidgetPrivate::mouseReleaseEvent(QMouseEvent *event)
{
  if(!Device) return;
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

  if(Device->postEventFromUser(e))
    event->accept();

  QWidget::mouseReleaseEvent(event);
}

void QIrrWidgetPrivate::wheelEvent(QWheelEvent *event)
{
  if(!Device) return;
  SEvent e;
  e.EventType = irr::EET_MOUSE_INPUT_EVENT;
  e.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
  e.MouseInput.X = event->x();
  e.MouseInput.Y = event->y();
  e.MouseInput.Wheel = event->delta()/qAbs(event->delta()); //Irrlicht uses values and 1 to -1. So let's just do that.
  if(Device->postEventFromUser(e))
    event->accept();

  QWidget::wheelEvent(event);
}

QIrrWidgetPrivate::QIrrWidgetPrivate( QIrrWidget *parent )
  : QWidget(parent),Device(0)
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
}

QIrrWidgetPrivate::~QIrrWidgetPrivate()
{
  killTimer(timerId);
  if ( Device != 0 )
    {
      Device->closeDevice();
      Device->drop();
      Device=0;
      QIrrWidget *parent=(QIrrWidget*)parentWidget();
      parent->Device=0;
    }
}

IrrlichtDevice* QIrrWidgetPrivate::initialize(irr::video::E_DRIVER_TYPE driverType)
{
  // Don't initialize more than once!
  if ( Device != 0 ) return Device;

  irr::SIrrlichtCreationParameters params;
  
  params.DriverType = driverType;
#ifdef Q_WS_X11
  params.WindowId = (void*)parentWidget()->winId();
#else
  params.WindowId = (void*)winId();
#endif
  params.WindowSize.Width = width();
  params.WindowSize.Height = height();
  params.AntiAlias = true;
  params.IgnoreInput = true;
  
  Device = irr::createDeviceEx( params );
  
#ifdef Q_WS_X11
  int _x=x(),_y=y();

  SExposedVideoData videoData=Device->getVideoDriver()->getExposedVideoData();
  WId window_id=videoData.OpenGLLinux.X11Window;
  create(window_id);
  move(_x,_y);
#endif
  
  timerId=startTimer(20);
  
  return Device;
}

void QIrrWidgetPrivate::timerEvent(QTimerEvent* event)
{
  update();
}
  
void QIrrWidgetPrivate::paintEvent( QPaintEvent* event )
{
  if(!Device)
    {
      QIrrWidget *parent=(QIrrWidget*)parentWidget();
      initialize(parent->driverType());
      Device->setEventReceiver((IEventReceiver*)parent);
      parent->Device=Device;
      parent->load();
    }
  
  if (Device)
    {
      Device->getTimer()->tick();
      //This disables rendering when we have a popup window, like when combining
      // tracks or loading an event. This hack is only enabled for OpenGL, because
      // I didn't notice any performance issues.
      //bool focusHack=(driverType()!=irr::video::EDT_OPENGL || QApplication::activeWindow()==window());
      if(isVisible() /*&& focusHack*/)
	{
	  ((QIrrWidget*)this->parentWidget())->execute();

	  irr::video::SColor color (0,0,0,0);
	  Device->getVideoDriver()->beginScene( isEnabled(), true, color );
	  Device->getSceneManager()->drawAll();
	  Device->getVideoDriver()->endScene();
	}
      else
	Device->yield();
    }

  QWidget::paintEvent(event);
}

void QIrrWidgetPrivate::resizeEvent( QResizeEvent* event )
{
    if ( Device != 0 )
    {
        irr::core::dimension2d<int> size;
        size.Width = event->size().width();
        size.Height = event->size().height();
        Device->getVideoDriver()->OnResize( size );

        irr::scene::ICameraSceneNode *cam = Device->getSceneManager()->getActiveCamera();
        if ( cam != 0 )
	  {
	    cam->setAspectRatio( size.Height / size.Width );
        }
    }
    QWidget::resizeEvent(event);
}

QPaintEngine * QIrrWidgetPrivate::paintEngine () const
{
  return 0;
}
