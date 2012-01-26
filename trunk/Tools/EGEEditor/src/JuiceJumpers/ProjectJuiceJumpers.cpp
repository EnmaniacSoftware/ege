#include "ProjectJuiceJumpers.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProjectJuiceJumpers::ProjectJuiceJumpers(QObject* parent, const QString& name, const QString& path) : Project(parent, name, path)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
ProjectJuiceJumpers::~ProjectJuiceJumpers()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Create method. */
Project* ProjectJuiceJumpers::Create(QObject* parent, const QString& name, const QString& path)
{
  return new ProjectJuiceJumpers(parent, name, path);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------