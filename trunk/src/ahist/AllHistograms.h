#ifndef ALLHISTOGRAMS_H_
#define ALLHISTOGRAMS_H_

#include <QMainWindow>

#include "AComboHistogram.h"

class AllHistograms : public QMainWindow
{
Q_OBJECT

 public:
  AllHistograms(QMenu *,QWidget *parent=0);
  QMenu* menu();

 public slots:
  void newHistorgamDlg();
  void addToHistogram();

 private:
  QTabWidget *tabWidget;
  QMenu *_menu;
  QAction *_sep;

  QMap<QString,AComboHistogram*> histograms;
};

#endif //ALLHISTOGRAMS_H_
