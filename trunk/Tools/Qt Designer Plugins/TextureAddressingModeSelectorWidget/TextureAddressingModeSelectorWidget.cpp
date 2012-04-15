#include <QtGui>
#include "TextureAddressingModeSelectorWidget.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAddressingModeSelectorWidget::TextureAddressingModeSelectorWidget(QWidget *parent) : QComboBox(parent)
{
  // populate
  addItem("Clamp", "clamp");
  addItem("Repeat", "repeat");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
