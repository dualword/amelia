#ifndef ACUSTOMPLAYER_H_
#define ACUSTOMPLAYER_H_

#ifdef QT_PHONON_LIB
#include <Phonon>
#else
#include <Phonon/VideoPlayer>
#endif

#include <QEvent>

class ACustomPlayer : public Phonon::VideoPlayer
{
public:
  ACustomPlayer(QWidget *parent);

  void changeEvent(QEvent *event);
};

#endif //ACUSTOMPLAYER_H_
