#include "resoucelibraryplugin.h"
#include <QtPlugin>
#include <QDebug>
#include <core.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::ResouceLibraryPlugin(QObject* parent) : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ResouceLibraryPlugin::~ResouceLibraryPlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool ResouceLibraryPlugin::initialize()
{
  Core* core = Core::instance();

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void ResouceLibraryPlugin::deinitialize()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN("resourcelibrary", ResouceLibraryPlugin, "core")
