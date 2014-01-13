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
  // disable focus proxy
  setFocusProxy(NULL);

  // open file selection dialog
  QString fileName = QFileDialog::getOpenFileName(this, m_title, QString(), m_filters);

  // check if any selection made and it is different
  if ( ! fileName.isEmpty() && (m_ui->label->text() != fileName))
  {
    // notify
    emit filePathChanged(fileName);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
