#include "JuiceJumpersProjectPlugin.h"
#include "ProjectJuiceJumpers.h"
#include <Core.h>
#include <ProjectFactory.h>
#include <QtPlugin>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
JuiceJumpersProjectPlugin::JuiceJumpersProjectPlugin(QObject* parent) : QObject(parent)
{
  qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
JuiceJumpersProjectPlugin::~JuiceJumpersProjectPlugin()
{
  qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool JuiceJumpersProjectPlugin::initialize()
{
  qDebug() << Q_FUNC_INFO;

  ProjectFactory* projectFactory = Core::instance()->projectFactory();
  if (projectFactory)
  {
    return projectFactory->registerProject(ProjectJuiceJumpers::TypeName, ProjectJuiceJumpers::Create);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Deinitializes plugin. */
void JuiceJumpersProjectPlugin::deinitialize()
{
  qDebug() << Q_FUNC_INFO;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN("juicejumpersproject", JuiceJumpersProjectPlugin, "core resourcelibrary")

