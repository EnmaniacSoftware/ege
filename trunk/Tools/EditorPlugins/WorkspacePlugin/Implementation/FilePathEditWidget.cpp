#include "FilePathEditWidget.h"
#include "ui_FilePathEditWidget.h"
#include <QFileDialog>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePathEditWidget::FilePathEditWidget(const QString& title, QWidget* parent) : QWidget(parent),
                                                                                m_ui(new Ui::FilePathEditWidget),
                                                                                m_title(title)
{
  m_ui->setupUi(this);

  // connect
  connect(m_ui->browse, SIGNAL(clicked()), this, SLOT(onBrowse()));

  // maker sure proxy is handles by browse button
  setFocusProxy(m_ui->browse);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePathEditWidget::~FilePathEditWidget()
{
  delete m_ui;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditWidget::setPath(const QString& path)
{
  m_ui->label->setText(path);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditWidget::setFilters(const QString& filters)
{
  m_filters = filters;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathEditWidget::onBrowse()
{
  // open file selection dialog
  QStringList list = QFileDialog::getOpenFileNames(this, m_title, QString(), m_filters);
  if ( ! list.isEmpty())
  {
    // go thru all items
    for (int i = 0; i < list.size(); ++i)
    {
      QString item = QDir::fromNativeSeparators(list[i]);
      QString name = item.section("/", -1);
      QString path = item.section("/", 0, -2);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
