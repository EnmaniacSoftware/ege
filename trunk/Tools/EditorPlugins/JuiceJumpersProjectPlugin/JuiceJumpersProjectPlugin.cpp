#include "JuiceJumpersProjectPlugin.h"
#include "ProjectJuiceJumpers.h"
#include <ProjectFactory.h>
#include <ObjectPool.h>
#include <QtPlugin>
#include <QDebug>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
JuiceJumpersProjectPlugin::JuiceJumpersProjectPlugin(QObject* parent) : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
JuiceJumpersProjectPlugin::~JuiceJumpersProjectPlugin()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! IPlugin override. Initialized plugin. */
bool JuiceJumpersProjectPlugin::initialize()
{
  ProjectFactory* projectFactory = ObjectPool::Instance()->getObject<ProjectFactory>();
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
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(JuiceJumpersProjectPlugin)

