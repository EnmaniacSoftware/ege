#include "BlendModeSelectorWidget.h"
#include "BlendModeSelectorWidgetPlugin.h"
#include <QtPlugin>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
BlendModeSelectorWidgetPlugin::BlendModeSelectorWidgetPlugin(QObject *parent) : QObject(parent)
{
  m_initialized = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void BlendModeSelectorWidgetPlugin::initialize(QDesignerFormEditorInterface* core)
{
  Q_UNUSED(core);

  if (m_initialized)
  {
    return;
  }

  m_initialized = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BlendModeSelectorWidgetPlugin::isInitialized() const
{
  return m_initialized;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* BlendModeSelectorWidgetPlugin::createWidget(QWidget* parent)
{
  return new BlendModeSelectorWidget(parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString BlendModeSelectorWidgetPlugin::name() const
{
  return "BlendModeSelectorWidget";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString BlendModeSelectorWidgetPlugin::group() const
{
  return "EGE Editor Widgets";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QIcon BlendModeSelectorWidgetPlugin::icon() const
{
  return QIcon();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString BlendModeSelectorWidgetPlugin::toolTip() const
{
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString BlendModeSelectorWidgetPlugin::whatsThis() const
{
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BlendModeSelectorWidgetPlugin::isContainer() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString BlendModeSelectorWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"BlendModeSelectorWidget\" name=\"BlendModeSelectorWidget\">\n"
           "  <property name=\"geometry\">\n"
           "   <rect>\n"
           "    <x>0</x>\n"
           "    <y>0</y>\n"
           "    <width>100</width>\n"
           "    <height>20</height>\n"
           "   </rect>\n"
           "  </property>\n"
           "  <property name=\"toolTip\" >\n"
           "   <string>The current time</string>\n"
           "  </property>\n"
           "  <property name=\"whatsThis\" >\n"
           "   <string>The blend mode selector widget enables selection of a blend mode.</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString BlendModeSelectorWidgetPlugin::includeFile() const
{
  return "BlendModeSelectorWidget.h";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(BlendModeSelectorWidgetPlugin, BlendModeSelectorWidgetPlugin)
