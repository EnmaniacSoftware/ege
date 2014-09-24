#ifndef EGE_CORE_ENGINE_ENGINEMODULE_H
#define EGE_CORE_ENGINE_ENGINEMODULE_H

/*! Base class for Engine modules. Module provides access to custom interface and implements this module at the same time.
 *  Module, being part of the engine, will be updated and notified about application state changes appropriately.
 *  Engine, on the other hand, will query module's state during its operation.
 */

#include "EGE.h"
#include "EGETime.h"
#include "Core/Engine/Interface/IEngineModule.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
class EngineModule : public IEngineModule
                   , public T
{
  public:

    EngineModule();
    virtual ~EngineModule();

  public:

    /*! Returns pointer to provided interface. */
    const T* iface() const;
    /*! Returns pointer to provided interface. */
    T* iface();

  protected:

    /*! @see IEngineModule::state. */
    EngineModuleState state() const override;
    /*! @see IEngineModule::constuct. */
    virtual EGEResult construct() override;
    /*! @see IEngineModule::onSuspend. */
    virtual void onSuspend() override;
    /*! @see IEngineModule::onResume. */
    virtual void onResume() override;
    /*! @see IEngineModule::onShutdown. */
    virtual void onShutdown() override;

    /*! Sets state. 
     *  @param  state New state to set.
     */
    void setState(EngineModuleState state);

  private:
    
    /*! Current state. */
    EngineModuleState m_state;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
EngineModule<T>::EngineModule()
: m_state(EModuleStateInvalid)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
EngineModule<T>::~EngineModule()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
EngineModuleState EngineModule<T>::state() const
{
  return m_state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void EngineModule<T>::setState(EngineModuleState state)
{
  m_state = state;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
const T* EngineModule<T>::iface() const
{
  return static_cast<const T*>(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
T* EngineModule<T>::iface()
{
  return static_cast<T*>(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
EGEResult EngineModule<T>::construct()
{
  m_state = EModuleStateRunning;
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void EngineModule<T>::onSuspend()
{
  m_state = EModuleStateSuspended;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void EngineModule<T>::onResume()
{
  m_state = EModuleStateRunning;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void EngineModule<T>::onShutdown()
{
  m_state = EModuleStateClosed;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_ENGINE_ENGINEMODULE_H