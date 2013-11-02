#include "JuiceJumpersProjectPlugin.h"
#include "JuiceJumpersProject.h"
#include <Projects/ProjectFactory.h>
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
bool JuiceJumpersProjectPlugin::initialize()
{
  // register project
  ProjectFactory* projectFactory = ObjectPool::Instance()->getObject<ProjectFactory>();
  if (NULL != projectFactory)
  {
    return projectFactory->registerProject(JuiceJumpersProject::TypeName, JuiceJumpersProject::Create);
  }

  return false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void JuiceJumpersProjectPlugin::deinitialize()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_EXPORT_PLUGIN(JuiceJumpersProjectPlugin)

