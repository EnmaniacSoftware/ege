#include "core.h"
#include "MainWindow.h"
#include "ProjectFactory.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Core::Core(QObject* parent) : QObject(parent), 
                              m_mainWindow(NULL),
                              m_projectFactory(NULL),
                              m_project(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Core::~Core()
{
  if (NULL != m_project)
  {
    delete m_project;
    m_project = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns core instance. */       
Core* Core::instance()
{
  static Core core;
  return &core;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Initializes object. */
bool Core::initialize()
{
  m_mainWindow = new MainWindow();
  m_projectFactory = new ProjectFactory();

  return m_mainWindow && m_projectFactory;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Deinitializes object. */
void Core::deinitialize()
{
  if (NULL != m_projectFactory)
  {
    delete m_projectFactory;
    m_projectFactory = NULL;
  }

  if (NULL != m_mainWindow)
  {
    delete m_mainWindow;
    m_mainWindow = NULL;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns main window. */
MainWindow* Core::mainWindow() const
{
  return m_mainWindow;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns project factory. */
ProjectFactory* Core::projectFactory() const
{
  return m_projectFactory;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns current project. */
Project* Core::currentProject() const
{
  return m_project;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when new project is created. */
void Core::onProjectCreated(Project* project)
{
  // store project
  m_project = project;

  emit projectCreated(project);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
    /* Returns main window. */
    QObject* Core::mainWindow2() const
    {
      return m_mainWindow;
    }
