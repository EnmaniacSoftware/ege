#ifndef EGE_CORE_RESOURCESOUND_H
#define EGE_CORE_RESOURCESOUND_H

/** Sound resource definition class. This object (resource) contains definition of sound.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGETime.h>
#include <EGEAudio.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceSound, PResourceSound)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceSound : public IResource
{
  public:

    virtual ~ResourceSound();

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

    /* Creates instance of sound object defined by resource. */
    PSound createInstance();

  private:

    ResourceSound(Application* app, ResourceManager* manager);
    /* Returns TRUE if object is loaded. */
    inline bool isLoaded() const { return true; }

  private:

    /*! Name. */
    String m_name;
    /*! Sound path. */
    String m_path;
    /*! Pitch value. */
    float32 m_pitch;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCESOUND_H
