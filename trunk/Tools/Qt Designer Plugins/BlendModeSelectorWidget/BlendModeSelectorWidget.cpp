#include <QtGui>
#include "BlendModeSelectorWidget.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BlendModeSelectorWidget::BlendModeSelectorWidget(QWidget *parent) : QComboBox(parent)
{
  // populate
  addItem("Zero", "zero");
  addItem("One", "one");
  addItem("Src-Color", "src-color");
  addItem("Dst-Color", "dst-color");
  addItem("One-Minus-Src-Color", "one-minus-src-color");
  addItem("One-Minus-Dst-Color", "one-minus-dst-color");
  addItem("Src-Alpha", "src-alpha");
  addItem("Dst-Alpha", "dst-alpha");
  addItem("One-Minus-Src-Alpha", "one-minus-src-alpha");
  addItem("One-Minus-Dst-Alpha", "one-minus-dst-alpha");
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
