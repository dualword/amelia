#ifndef ALABNOTEBOOK_H_
#define ALABNOTEBOOK_H_

#include <APlugin.h>

class ALabNoteBook : public QObject, public APlugin
{
  Q_OBJECT
  Q_INTERFACES(APlugin)

public:
  ALabNoteBook ( QObject *parent=0 );
  ~ALabNoteBook();
  
  void load();
};

#endif // ALABNOTEBOOK_H_

