#include "Core/Input/Pointer.h"
#include "EGEDebug.h"

#ifdef EGE_PLATFORM_WIN32
  #include "Win32/Input/PointerWin32_p.h"
#elif EGE_PLATFORM_AIRPLAY
  #include "Airplay/Input/PointerAirplay_p.h"
#elif EGE_PLATFORM_IOS
  #include "iOS/Input/PointerIOS_p.h"
#endif

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Pointer)
EGE_DEFINE_DELETE_OPERATORS(Pointer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Pointer::Pointer(Engine& engine)
: m_engine(engine)
{
  m_p = ege_new PointerPrivate(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Pointer::~Pointer()
{
  EGE_DELETE(m_p);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& Pointer::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END