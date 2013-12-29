#include "TextureSettingsWidget.h"
#include "TextureSettingsWidgetPlugin.h"
#include <QtPlugin>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureSettingsWidgetPlugin::TextureSettingsWidgetPlugin(QObject *parent) : QObject(parent)
{
  m_initialized = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextureSettingsWidgetPlugin::initialize(QDesignerFormEditorInterface* core)
{
  Q_UNUSED(core);

  if (m_initialized)
  {
    return;
  }

  m_initialized = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TextureSettingsWidgetPlugin::isInitialized() const
{
  return m_initialized;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* TextureSettingsWidgetPlugin::createWidget(QWidget* parent)
{
  return new TextureSettingsWidget(parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureSettingsWidgetPlugin::name() const
{
  return "TextureSettingsWidget";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureSettingsWidgetPlugin::group() const
{
  return "EGE Editor Widgets";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QIcon TextureSettingsWidgetPlugin::icon() const
{
  return QIcon();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureSettingsWidgetPlugin::toolTip() const
{
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureSettingsWidgetPlugin::whatsThis() const
{
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TextureSettingsWidgetPlugin::isContainer() const
{
  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureSettingsWidgetPlugin::domXml() const
{
  return "<ui language=\"c++\">\n"
         " <widget class=\"TextureSettingsWidget\" name=\"textureSettings\">\n"
         "  <property name=\"geometry\">\n"
         "   <rect>\n"
         "    <x>0</x>\n"
         "    <y>0</y>\n"
         "    <width>100</width>\n"
         "    <height>20</height>\n"
         "   </rect>\n"
         "  </property>\n"
         "  <property name=\"toolTip\" >\n"
         "   <string></string>\n"
         "  </property>\n"
         "  <property name=\"whatsThis\" >\n"
         "   <string>The texture settings widget enables setup of common texture properies.</string>\n"
         "  </property>\n"
         " </widget>\n"
         "</ui>\n";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureSettingsWidgetPlugin::includeFile() const
{
  return "TextureSettingsWidget.h";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

