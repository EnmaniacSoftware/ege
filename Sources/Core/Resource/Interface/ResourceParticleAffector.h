#ifndef EGE_CORE_RESOURCEPARTICLEAFFECTOR_H
#define EGE_CORE_RESOURCEPARTICLEAFFECTOR_H

/** Particle affector resource definition class. This object (resource) contains definition of particle affector.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGETime.h"
#include "EGEDictionary.h"
#include "EGEParticle.h"
#include "Core/Resource/Interface/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceParticleAffector, PResourceParticleAffector)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceParticleAffector : public IResource
{
  public:

    virtual ~ResourceParticleAffector();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Engine& engine, ResourceGroup* group);

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PXmlElement& tag) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Creates instance of particle affector object defined by resource. */
    PParticleAffector createInstance();

  private:

    ResourceParticleAffector(Engine& engine, ResourceGroup* group);

  private:

    /*! Name. */
    String m_name;
    /*! Dictionary with defined parameters. */
    Dictionary m_parameters;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEPARTICLEAFFECTOR_H
