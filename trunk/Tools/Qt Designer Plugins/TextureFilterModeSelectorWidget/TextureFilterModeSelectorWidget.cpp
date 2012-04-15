#include <QtGui>
#include "TextureFilterModeSelectorWidget.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureFilterModeSelectorWidget::TextureFilterModeSelectorWidget(QWidget *parent) : QComboBox(parent)
{
  // populate
  addItem("Bilinear", "bilinear");
  addItem("Trilinear", "trilinear");
  addItem("Mip-Mapping bilinear", "mipmap-bilinear");
  addItem("Mip-Mapping trilinear", "mipmap-trilinear");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
