#include <QtGui>
#include "TextureEnvironmentModeSelectorWidget.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureEnvironmentModeSelectorWidget::TextureEnvironmentModeSelectorWidget(QWidget *parent) : QComboBox(parent)
{
  // populate
  addItem("Replace", "replace");
  addItem("Modulate", "modulate");
  addItem("Decal", "decal");
  addItem("Blend", "blend");
  addItem("Add", "add");
  //addItem("Combine", "combine");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
