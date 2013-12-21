#include <QtGui>
#include "TextureSettingsWidget.h"
#include "ui_widget.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureSettingsWidget::TextureSettingsWidget(QWidget *parent) : QWidget(parent),
                                                                 m_ui(new Ui_TextureSettings())
{
  // setup UI
  m_ui->setupUi(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
