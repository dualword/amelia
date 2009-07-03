#include "AGeometryHUD.h"

AGeometryHUD::AGeometryHUD(AGeometry *geo)
  :geo(geo),textNode(0)
{ 
  connect(geo,SIGNAL(finishedLoading()),
	  this,SLOT(setupElements()));
  connect(geo,SIGNAL(cameraSwitched(int)),
	  this,SLOT(handleNewCamera(int)));
  connect(geo,SIGNAL(cameraControlSwitched(bool)),
	  this,SLOT(handleCameraControl(bool)));

  geo->installEventFilter(this);
}

void AGeometryHUD::setupElements()
{
  gui=geo->getGUIEnvironment();
  
  core::dimension2d<u32> size=geo->getVideoDriver()->getScreenSize();
  
  core::rect<f32> textRect(0.2,0.9,0.8,1.0);
  
  textNode=gui->addStaticText(L"",
			      core::rect<s32>(0,0,0,0));
  textNode->setRelativePositionProportional(textRect);
  textNode->setOverrideColor(SColor(255,255,255,255));
  textNode->setTextAlignment(EGUIA_CENTER,EGUIA_SCALE);
  textNode->setEnabled(false);
}

void AGeometryHUD::setText(QString text)
{ 
  textNode->setText(text.toStdWString().c_str());
  geo->makeDirty();
}

void AGeometryHUD::clearText()
{ 
  if(textNode)
    {
      textNode->setText(L"");
      geo->makeDirty();
    }
}

void AGeometryHUD::handleNewCamera(int camid)
{
  if(camid==AGeometry::FPS)
    setText("Press SPACE or double click to engage camera");
  else
    clearText();
}

void AGeometryHUD::handleCameraControl(bool grabbed)
{
  if(grabbed)
    setText("Press SPACE or double click to release camera");
  else
    setText("Press SPACE or double click to engage camera");
}

