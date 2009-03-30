#ifndef ALABNOTEBOOKWIDGET_H_
#define ALABNOTEBOOKWIDGET_H_

#include <QWidget>
#include <QPushButton>
#include <QListView>
#include <QPlainTextEdit>
#include <QLabel>

#include <aeventmanager/AEvent.h>

#include "ALabNoteBookTableModel.h"

class ALabNoteBookWidget : public QWidget
{
public:
  ALabNoteBookWidget(QWidget* parent=0);
  ~ALabNoteBookWidget();
  
  void setupElements();

public slots:
  void handleNewEventLoaded(AEvent* event);
  void createNewNote();
  void handleNoteSelected(const QModelIndex& index);
  void handleNoteEdited();
  
private:
  ALabNoteBookTableModel* _tableModel;
  ALabNoteBookData* _data;
  ALabNoteBookEntry* _activeEntry;
  
  QPushButton* _newEntryButton;
  QListView* _notesListTable;
  QPlainTextEdit *_noteTextEdit;
  QLabel *_createTimeLabel;
  Q_OBJECT
};

#endif //ALABNOTEBOOKWIDGET_H_
