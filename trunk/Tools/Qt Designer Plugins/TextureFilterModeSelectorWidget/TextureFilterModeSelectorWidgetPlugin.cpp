#include "TextureFilterModeSelectorWidget.h"
#include "TextureFilterModeSelectorWidgetPlugin.h"
#include <QtPlugin>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureFilterModeSelectorWidgetPlugin::TextureFilterModeSelectorWidgetPlugin(QObject *parent) : QObject(parent)
{
  m_initialized = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextureFilterModeSelectorWidgetPlugin::initialize(QDesignerFormEditorInterface* core)
{
  Q_UNUSED(core);

  if (m_initialized)
  {
    return;
  }

  m_initialized = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TextureFilterModeSelectorWidgetPlugin::isInitialized() const
{
  return m_initialized;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* TextureFilterModeSelectorWidgetPlugin::createWidget(QWidget* parent)
{
  return new TextureFilterModeSelectorWidget(parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureFilterModeSelectorWidgetPlugin::name() const
{
  return "TextureFilterModeSelectorWidget";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureFilterModeSelectorWidgetPlugin::group() const
{
  return "EGE Editor Widgets";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QIcon TextureFilterModeSelectorWidgetPlugin::icon() const
{
  return QIcon();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureFilterModeSelectorWidgetPlugin::toolTip() const
{
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureFilterModeSelectorWidgetPlugin::whatsThis() const
{
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TextureFilterModeSelectorWidgetPlugin::isContainer() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureFilterModeSelectorWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"TextureFilterModeSelectorWidget\" name=\"TextureFilterModeSelectorWidget\">\n"
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
           "   <string>The texture filter mode selector widget enables selection ofa texture filtering mode.</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureFilterModeSelectorWidgetPlugin::includeFile() const
{
  return "TextureFilterModeSelectorWidget.h";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(TextureFilterModeSelectorWidgetPlugin, TextureFilterModeSelectorWidgetPlugin)
