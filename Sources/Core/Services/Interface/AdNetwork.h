#ifndef EGE_CORE_SERVICES_ADNETWORK_H
#define EGE_CORE_SERVICES_ADNETWORK_H

/*! Base class for specific Advertisement Network soulutions.
 */

#include "EGE.h"
#include "EGEList.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
extern const char* KDefaultAdNetworkName;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(AdNetwork, PAdNetwork)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AdNetwork : public Object
{
  public:

    AdNetwork(Engine& engine);
    virtual ~AdNetwork();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS
  
  public:
  
    /*! Returns pointer to application object. */
    Engine& engine() const;

  private:
  
    /*! Reference to engine. */
    Engine& m_engine;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef List<AdNetwork*> AdNetworkList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SERVICES_ADNETWORK_H
