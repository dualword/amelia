#include "ASnapshotTool.h"

#include <QFileDialog>

ASnapshotTool::ASnapshotTool(QObject *parent)
  :QObject(parent),_target(0),_geotarget(0)
{
  connect(&fader,SIGNAL(fadeFinished()),
	  this,SLOT(finish()));
 }

void ASnapshotTool::setTarget(QWidget *target)
{
  _target=target;
  _geotarget=qobject_cast<AGeometry*>(_target);

  fader.setParent(_target);
  fader.hide();
}

QWidget* ASnapshotTool::target()
{
  return _target;
}

void ASnapshotTool::snapshot()
{
  if(!_target) return;

  
  // Switch to the locked camera, without any controls 
  // if we are in 3D mode.
  if(_geotarget && _geotarget->viewport()==AGeometry::Cam3D)
    {
      APoint3D currentPos=_geotarget->cameraPosition();
      APoint3D currentTar=_geotarget->cameraTarget();
      currentCam=_geotarget->camera();

      _geotarget->setCamera(AGeometry::Lock,false);
      _geotarget->setCameraPosition(currentPos);
      _geotarget->setCameraTarget(currentTar);
      _geotarget->forceUpdate();
    }

  // Take screenshot
  lastSnapshot=QPixmap::grabWindow(_target->winId());
  _target->setEnabled(false);

  fader.show();
}

void ASnapshotTool::finish()
{
  QString fileName=QFileDialog::getSaveFileName(_target,"Save To",QString(),"Images (*.png *.xpm *.jpg)");
  if(!fileName.isEmpty())
    lastSnapshot.save(fileName);

  // Restore the camera, if in 3D
  if(_geotarget && _geotarget->viewport()==AGeometry::Cam3D)
    {
      _geotarget->setCamera(currentCam,false);
    }
  _target->setEnabled(true);
}
