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
  if(_geotarget)
    {
      _geotarget->lockCamera();
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
  if(_geotarget)
    _geotarget->unlockCamera();

  _target->setEnabled(true);
}
