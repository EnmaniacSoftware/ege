#include "TextureAddressingModeSelectorWidget.h"
#include "TextureAddressingModeSelectorWidgetPlugin.h"
#include <QtPlugin>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
TextureAddressingModeSelectorWidgetPlugin::TextureAddressingModeSelectorWidgetPlugin(QObject *parent) : QObject(parent)
{
  m_initialized = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void TextureAddressingModeSelectorWidgetPlugin::initialize(QDesignerFormEditorInterface* core)
{
  Q_UNUSED(core);

  if (m_initialized)
  {
    return;
  }

  m_initialized = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TextureAddressingModeSelectorWidgetPlugin::isInitialized() const
{
  return m_initialized;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* TextureAddressingModeSelectorWidgetPlugin::createWidget(QWidget* parent)
{
  return new TextureAddressingModeSelectorWidget(parent);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureAddressingModeSelectorWidgetPlugin::name() const
{
  return "TextureAddressingModeSelectorWidget";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureAddressingModeSelectorWidgetPlugin::group() const
{
  return "EGE Editor Widgets";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QIcon TextureAddressingModeSelectorWidgetPlugin::icon() const
{
  return QIcon();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureAddressingModeSelectorWidgetPlugin::toolTip() const
{
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureAddressingModeSelectorWidgetPlugin::whatsThis() const
{
  return "";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TextureAddressingModeSelectorWidgetPlugin::isContainer() const
{
  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureAddressingModeSelectorWidgetPlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
           " <widget class=\"TextureAddressingModeSelectorWidget\" name=\"TextureAddressingModeSelectorWidget\">\n"
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
           "   <string>The texture addressing mode selector widget enables selection of a texture addressing mode.</string>\n"
           "  </property>\n"
           " </widget>\n"
           "</ui>\n";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString TextureAddressingModeSelectorWidgetPlugin::includeFile() const
{
  return "TextureAddressingModeSelectorWidget.h";
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(TextureAddressingModeSelectorWidgetPlugin, TextureAddressingModeSelectorWidgetPlugin)
