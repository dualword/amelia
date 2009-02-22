#ifndef ATEST_H_
#define ATEST_H_

#include <QWidget>
#include <QTabWidget>
#include <QMap>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include <TH1F.h>
#include <TQtWidget.h>
#include <TCanvas.h>

#include <AMELIA.h>
#include <aeventmanager/AEvent.h>

class ATest : public QWidget
{
Q_OBJECT

 public:
  ATest(QWidget* parent=0);
  
 public slots:
  void updateBins();
  void loadEvent(AEvent*);
  
 private:
  QSpinBox *bins;
  QDoubleSpinBox *minx;
  QDoubleSpinBox *maxx;
  QTabWidget *tabs;
  
  QMap<AEvent*,TH1F*> hists;
  QMap<AEvent*,TQtWidget*> widgets;
};

#endif //ATEST_H_
