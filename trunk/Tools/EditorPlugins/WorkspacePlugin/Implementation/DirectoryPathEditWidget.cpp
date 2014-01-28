#include "DirectoryPathEditWidget.h"
#include "ui_FilePathEditWidget.h"
#include <QFileDialog>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DirectoryPathEditWidget::DirectoryPathEditWidget(const QString& title, QWidget* parent) : QWidget(parent),
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
DirectoryPathEditWidget::~DirectoryPathEditWidget()
{
  delete m_ui;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DirectoryPathEditWidget::setPath(const QString& path)
{
  m_ui->label->setText(path);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DirectoryPathEditWidget::onBrowse()
{
  // disable focus proxy
  setFocusProxy(NULL);

  // open directory selection dialog
  QString directoryName = QFileDialog::getExistingDirectory(this, m_title);

  // check if any selection made and it is different
  if ( ! directoryName.isEmpty() && (m_ui->label->text() != directoryName))
  {
    // notify
    emit directoryPathChanged(directoryName);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
