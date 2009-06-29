#ifndef AGEOMETRYHUD_H_
#define AGEOMETRYHUD_H_

#include "AGeometry.h"

class AGeometryHUD : public QObject
{
 public:
  AGeometryHUD(AGeometry *geo);

 public slots:
  void setupElements();
  void setText(QString text);
  void clearText();

  void handleNewCamera(int camid);
  void handleCameraControl(bool grabbed);
  
 private:
  AGeometry *geo;
  IGUIEnvironment *gui;

  gui::IGUIStaticText *textNode;

  Q_OBJECT
};

#endif // AGEOMETRYHUD_H_
