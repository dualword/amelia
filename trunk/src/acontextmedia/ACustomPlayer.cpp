#include "ACustomPlayer.h"
#include <QDebug>

ACustomPlayer::ACustomPlayer(QWidget *parent)
  : Phonon::VideoPlayer(parent)
{ }

void ACustomPlayer::changeEvent(QEvent *event)
{
  if(event->type()==QEvent::EnabledChange)
    {
      if(isEnabled())
	play();
      else
	stop();
    }
}
