#include <QtGui>
#include "TextureSettingsWidget.h"
#include "ui_widget.h"
#include <qtpropertymanager.h>
#include <qteditorfactory.h>
#include <qttreepropertybrowser.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureSettingsWidget::TextureSettingsWidget(QWidget *parent) : QWidget(parent),
                                                                 m_ui(new Ui_TextureSettings())
{
  // setup UI
  m_ui->setupUi(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
