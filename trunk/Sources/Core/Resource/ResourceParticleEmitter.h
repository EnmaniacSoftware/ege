#ifndef EGE_CORE_RESOURCEPARTICLEEMITTER_H
#define EGE_CORE_RESOURCEPARTICLEEMITTER_H

/** Particle emitter resource definition class. This object (resource) contains definition of particle emitter.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGETime.h>
#include <EGEDictionary.h>
#include <EGEParticle.h>
#include <EGEStringList.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceParticleEmitter, PResourceParticleEmitter)
EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceParticleEmitter : public IResource
{
  public:

    virtual ~ResourceParticleEmitter();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceGroup* group);

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PXmlElement& tag) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Creates instance of particle emitter object defined by resource. */
    PParticleEmitter createInstance();

  private:

    ResourceParticleEmitter(Application* app, ResourceGroup* group);
    /*! Adds affector. */
    EGEResult addAffector(const PXmlElement& tag);

  private:

    /*! Name. */
    String m_name;
    /*! Dictionary with defined parameters. */
    Dictionary m_parameters;
    /*! Material resource. NULL if not loaded yet. */
    PResourceMaterial m_materialResource;
    /*! List of affectors. */
    StringList m_affectors;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEPARTICLEEMITTER_H
