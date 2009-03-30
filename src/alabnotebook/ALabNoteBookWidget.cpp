#include "ALabNoteBookWidget.h"
#include <QDebug>

ALabNoteBookWidget::ALabNoteBookWidget(QWidget* parent):QWidget(parent)
{ 
  _tableModel=new ALabNoteBookTableModel(this);
  _activeEntry=0;
}

ALabNoteBookWidget::~ALabNoteBookWidget()
{ }

void ALabNoteBookWidget::setupElements()
{
  setEnabled(false);

  _newEntryButton=findChild<QPushButton*>("newEntryButton");
  _notesListTable=findChild<QListView*>("notesListTable");
  _noteTextEdit=findChild<QPlainTextEdit*>("noteTextEdit");
  _createTimeLabel=findChild<QLabel*>("createTimeLabel");

  if(_notesListTable)
    {
      _notesListTable->setModel(_tableModel);
      connect(_notesListTable,SIGNAL(activated(const QModelIndex&)),
	      this,SLOT(handleNoteSelected(const QModelIndex&)));
    }

  if(_newEntryButton)
    {
      connect(_newEntryButton,SIGNAL(clicked()),
	      this,SLOT(createNewNote()));
    }
  
  if(_noteTextEdit)
    {
      connect(_noteTextEdit,SIGNAL(textChanged()),
	      this,SLOT(handleNoteEdited()));
    }
}

void ALabNoteBookWidget::handleNewEventLoaded(AEvent* event)
{
  _data=event->getAnalysisData<ALabNoteBookData>("ALabNoteBook");
  _tableModel->setLabNoteBookData(_data);
  handleNoteSelected(QModelIndex());
  
  setEnabled(true);
}

void ALabNoteBookWidget::createNewNote()
{
  _data->addEntry(new ALabNoteBookEntry());
}

void ALabNoteBookWidget::handleNoteSelected(const QModelIndex& index)
{
  QVariant data=_tableModel->data(index,Qt::UserRole);
  ALabNoteBookEntry* entry=(ALabNoteBookEntry*)data.value<QObject*>();

  _activeEntry=0;
  QString text="";
  QString timeText="";

  if(entry)
    {
      text=entry->text();
      timeText=entry->time().toString("yyyy-mm-dd h:m:s ap");
      _activeEntry=entry;
      _noteTextEdit->setEnabled(true);
    }
  else
    {
      _noteTextEdit->setEnabled(false);
    }

  _tableModel->setActiveEntry(_activeEntry);
  _noteTextEdit->setPlainText(text);
  _createTimeLabel->setText(timeText);
}

void ALabNoteBookWidget::handleNoteEdited()
{
  if(_activeEntry)
    {
      _activeEntry->setText(_noteTextEdit->toPlainText());
    }
}
