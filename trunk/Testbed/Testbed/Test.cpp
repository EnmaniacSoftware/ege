#include "Test.h"
#include "App.h"
#include <EGEResources.h>

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Test::Test(App* app) : m_app(app)
{
  ege_connect(app->resourceManager(), groupLoadComplete, this, Test::groupLoadComplete);
  ege_connect(app->resourceManager(), groupLoadError, this, Test::groupLoadError);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Test::~Test()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Pointer event receiver. */
void Test::pointerEvent(PPointerData data)
{
  EGE_UNUSED(data);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when resource group has been loaded. */
void Test::groupLoadComplete(const String& name)
{
  EGE_UNUSED(name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Slot called when resource group could not be loaded. */
void Test::groupLoadError(const String& name)
{
  EGE_UNUSED(name);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
