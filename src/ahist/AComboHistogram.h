#ifndef ACOMBOHISTOGRAM_H_
#define ACOMBOHISTOGRAM_H_

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

class AComboHistogram : public QWidget
{
Q_OBJECT

 public:
  AComboHistogram(QString name,QWidget* parent=0);

  QString name;
  
 public slots:
  void updateBins();
  void addData(int);
  
 private:
  QSpinBox *bins;
  QDoubleSpinBox *minx;
  QDoubleSpinBox *maxx;
  
  TH1F* hist;
  TQtWidget* widget;

  QList<int> data;
};

#endif //ACOMBOHISTOGRAM_H_
