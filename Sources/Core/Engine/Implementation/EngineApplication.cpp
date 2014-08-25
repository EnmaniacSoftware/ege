#include "Core/Engine/Interface/EngineApplication.h"
#include "Core/Engine/Interface/EngineInternal.h"
#include "Core/Engine/Interface/EngineInstance.h"
#include "EGEEngine.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(EngineApplication)
EGE_DEFINE_DELETE_OPERATORS(EngineApplication)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineApplication::EngineApplication(Engine& engine) : m_engine(engine)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EngineApplication::~EngineApplication()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void EngineApplication::close()
{
  EngineInternal& engineInternal = static_cast<EngineInternal&>(reinterpret_cast<EngineInstance&>(m_engine));
  engineInternal.shutdown();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& EngineApplication::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END