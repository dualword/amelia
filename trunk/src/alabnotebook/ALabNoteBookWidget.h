#ifndef ALABNOTEBOOKWIDGET_H_
#define ALABNOTEBOOKWIDGET_H_

#include <QWidget>
#include <QPushButton>
#include <QListView>
#include <QPlainTextEdit>
#include <QLabel>
#include <QWebView>

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

  /* Edit action slots */
  void insertImage();

  void adjustButtons();

  void execCommand(const QString &cmd, const QString &arg);
  void execCommand(const QString &cmd);
  
private:
  ALabNoteBookTableModel* _tableModel;
  ALabNoteBookData* _data;
  ALabNoteBookEntry* _activeEntry;
  
  QPushButton* _newEntryButton;

  QPushButton* _insertImageButton;
  QPushButton* _boldButton;
  QPushButton* _italicButton;
  QPushButton* _underlineButton;

  QListView* _notesListTable;
  QWebView *_noteTextEdit;
  QLabel *_createTimeLabel;

  Q_OBJECT
};

#endif //ALABNOTEBOOKWIDGET_H_
