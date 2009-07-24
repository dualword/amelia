#include "ALabNoteBookWidget.h"
#include <QDebug>
#include <QWebFrame>
#include <QFileDialog>
#include <QFile>
#include <QAction>

/* Macros borrowed from Qt's HTML editor example */
#define FOLLOW_ENABLE(a1, a2) a1->setEnabled(_noteTextEdit->pageAction(a2)->isEnabled())
#define FOLLOW_CHECK(a1, a2) a1->setChecked(_noteTextEdit->pageAction(a2)->isChecked())

#define FORWARD_BUTTON(action1, action2)  \
  connect(action1, SIGNAL(clicked()),				   \
	  _noteTextEdit->pageAction(action2), SLOT(trigger()));	   \
  connect(_noteTextEdit->pageAction(action2), SIGNAL(changed()),   \
	  this,SLOT(adjustButtons()));


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
  _noteTextEdit=findChild<QWebView*>("noteTextEdit");
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
      connect(_noteTextEdit->page(),SIGNAL(contentsChanged()),
	      this,SLOT(handleNoteEdited()));
    }

  /* Prepare the actions */
  _insertImageButton=findChild<QPushButton*>("insertImageButton");
  _boldButton=findChild<QPushButton*>("boldButton");
  _italicButton=findChild<QPushButton*>("italicButton");
  _underlineButton=findChild<QPushButton*>("underlineButton");

  if(_insertImageButton)
    {
      connect(_insertImageButton,SIGNAL(clicked()),
	      this,SLOT(insertImage()));
    }


  if(_boldButton)
    {
      FORWARD_BUTTON(_boldButton, QWebPage::ToggleBold);
    }
  if(_italicButton)
    {
      FORWARD_BUTTON(_italicButton, QWebPage::ToggleItalic);
    }
  if(_underlineButton)
    {
      FORWARD_BUTTON(_underlineButton, QWebPage::ToggleUnderline);
    }
}

void ALabNoteBookWidget::handleNewEventLoaded(AEvent* event)
{
  if(event)
    {
      _data=event->getAnalysisData<ALabNoteBookData>("ALabNoteBook");
      setEnabled(true);
    }
  else
    {
      _data=0;
      setEnabled(false);
    }
  
  _tableModel->setLabNoteBookData(_data);
  handleNoteSelected(QModelIndex());
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
  _noteTextEdit->setHtml(text);
  _noteTextEdit->page()->setContentEditable(true);
  _createTimeLabel->setText(timeText);
}

void ALabNoteBookWidget::handleNoteEdited()
{
  if(_activeEntry)
    {
      _activeEntry->setText(_noteTextEdit->page()->mainFrame()->toHtml());
    }
}

void ALabNoteBookWidget::insertImage()
{
  QString filters;
  filters += tr("Common Graphics (*.png *.jpg *.jpeg *.gif);;");
  filters += tr("Portable Network Graphics (PNG) (*.png);;");
  filters += tr("JPEG (*.jpg *.jpeg);;");
  filters += tr("Graphics Interchange Format (*.gif);;");
  filters += tr("All Files (*)");
  
  QString fn = QFileDialog::getOpenFileName(this, tr("Open image..."),
					    QString(), filters);
  if (fn.isEmpty())
    return;
  if (!QFile::exists(fn))
    return;
  
  QUrl url = QUrl::fromLocalFile(fn);
  execCommand("insertImage", url.toString());
}

void ALabNoteBookWidget::adjustButtons()
{
  FOLLOW_CHECK(_boldButton,QWebPage::ToggleBold);
  FOLLOW_CHECK(_italicButton,QWebPage::ToggleItalic);
  FOLLOW_CHECK(_underlineButton,QWebPage::ToggleUnderline);
}

void ALabNoteBookWidget::execCommand(const QString &cmd, const QString &arg)
{
  // How execCommand works:
  // http://msdn.microsoft.com/en-us/library/ms536419(VS.85).aspx
  // Available Commands
  // http://msdn.microsoft.com/en-us/library/ms533049(VS.85).aspx
  
  QString js = QString("document.execCommand(\"%1\", false, \"%2\")").arg(cmd).arg(arg);
  QWebFrame *frame = _noteTextEdit->page()->mainFrame();
  frame->evaluateJavaScript(js);
}

void ALabNoteBookWidget::execCommand(const QString &cmd)
{
  // How execCommand works:
  // http://msdn.microsoft.com/en-us/library/ms536419(VS.85).aspx
  // Available Commands
  // http://msdn.microsoft.com/en-us/library/ms533049(VS.85).aspx

  QString js = QString("document.execCommand(\"%1\", false,null").arg(cmd);
  QWebFrame *frame = _noteTextEdit->page()->mainFrame();
  frame->evaluateJavaScript(js);
}
