#include "TextureEnvironmentModeSelectorWidget.h"
#include "TextureEnvironmentModeSelectorWidgetPlugin.h"
#include <QtPlugin>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureEnvironmentModeSelectorWidgetPlugin::TextureEnvironmentModeSelectorWidgetPlugin(QObject *parent) : QObject(parent)
{
  m_initialized = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextureEnvironmentModeSelectorWidgetPlugin::initialize(QDesignerFormEditorInterface* core)
{
  Q_UNUSED(core);

  if (m_initialized)
  {
    return;
  }

  m_initialized = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TextureEnvironmentModeSelectorWidgetPlugin::isInitialized() const
{
  return m_initialized;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* TextureEnvironmentModeSelectorWidgetPlugin::createWidget(QWidget* parent)
{
  return new TextureEnvironmentModeSelectorWidget(parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureEnvironmentModeSelectorWidgetPlugin::name() const
{
  return "TextureEnvironmentModeSelectorWidget";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureEnvironmentModeSelectorWidgetPlugin::group() const
{
  return "EGE Editor Widgets";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QIcon TextureEnvironmentModeSelectorWidgetPlugin::icon() const
{
  return QIcon();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureEnvironmentModeSelectorWidgetPlugin::toolTip() const
{
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureEnvironmentModeSelectorWidgetPlugin::whatsThis() const
{
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TextureEnvironmentModeSelectorWidgetPlugin::isContainer() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureEnvironmentModeSelectorWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"TextureEnvironmentModeSelectorWidget\" name=\"TextureEnvironmentModeSelectorWidget\">\n"
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
           "   <string>The texture environment mode selector widget enables selection ofa texture environment mode.</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureEnvironmentModeSelectorWidgetPlugin::includeFile() const
{
  return "TextureEnvironmentModeSelectorWidget.h";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(TextureEnvironmentModeSelectorWidgetPlugin, TextureEnvironmentModeSelectorWidgetPlugin)
