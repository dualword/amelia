#ifndef ATOUR_H_
#define ATOUR_H_

#include "ATourBlock.h"

#include <QString>
#include <QDomElement>

class ATour
{
 public:
  static enum {AT_PLAYSOUND, AT_BUTTON, AT_LOADFILE, AT_CAMMODE, AT_PTCHANGE, AT_TRACKSEL} TOUR_ACTIONS;
  
  ATour(QString path);

  QString title();
  QString description();

  ATourBlock *block(int);
  int blockCount();

 private:
  QString _title;
  QString _description;
  QString _location;

  QList<ATourBlock*> blocks;
  
  void loadTour();
  void loadBlock(QDomElement blockElement);
};

#endif //ATOUR_H_
