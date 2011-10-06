#ifndef EGE_CORE_RESOURCEPARTICLEAFFECTOR_H
#define EGE_CORE_RESOURCEPARTICLEAFFECTOR_H

/** Particle affector resource definition class. This object (resource) contains definition of particle affector.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGETime.h>
#include <EGEDictionary.h>
#include <EGEParticle.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceParticleAffector, PResourceParticleAffector)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceParticleAffector : public IResource
{
  public:

    virtual ~ResourceParticleAffector();

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

    /* Creates instance of particle affector object defined by resource. */
    PParticleAffector createInstance();

  private:

    ResourceParticleAffector(Application* app, ResourceManager* manager);
    /* Returns TRUE if object is loaded. */
    inline bool isLoaded() const { return true; }

  private:

  private:

    /*! Name. */
    String m_name;
    /*! Dictionary with defined parameters. */
    Dictionary m_parameters;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEPARTICLEAFFECTOR_H
