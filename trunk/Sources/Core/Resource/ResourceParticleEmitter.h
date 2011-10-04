#ifndef EGE_CORE_RESOURCEPARTICLEEMITTER_H
#define EGE_CORE_RESOURCEPARTICLEEMITTER_H

/** Particle emitter resource definition class. This object (resource) contains definition of particle emitter.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGETime.h>
#include <EGEDictionary.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceParticleEmitter, PResourceParticleEmitter)
EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceParticleEmitter : public IResource
{
  public:

    virtual ~ResourceParticleEmitter();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceManager* manager);

    /* IResource override. Returns name of resource. */
    const String& name() const override;
    /* Initializes resource from XML. 
    * 
    *  \param  path  full path to resource definition file.
    *  \param  tag   xml element with resource definition. 
    */
    EGEResult create(const String& path, const PXmlElement& tag) override;
    /* IResource override. Loads resource. */
    EGEResult load() override;
    /* IResource override. Unloads resource. */
    void unload() override;

    /* Creates instance of sprite object defined by resource. */
    //PSprite createInstance();
    /* Set given instance of sprite object to what is defined by resource. */
    //EGEResult setInstance(const PSprite& instance);

  private:

    ResourceParticleEmitter(Application* app, ResourceManager* manager);
    /* Returns TRUE if object is loaded. */
    inline bool isLoaded() const { return (NULL != m_materialResource); }

  private:

  private:

    /*! Name. */
    String m_name;
    /*! Dictionary with defined parameters. */
    Dictionary m_parameters;
    /*! Material resource. */
    PResourceMaterial m_materialResource;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEPARTICLEEMITTER_H
