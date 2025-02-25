#ifndef EGE_CORE_RESOURCESOUND_H
#define EGE_CORE_RESOURCESOUND_H

/** Sound resource definition class. This object (resource) contains definition of sound.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGETime.h"
#include "EGEAudio.h"
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceSound, PResourceSound)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceSound : public IResource
{
  public:

    virtual ~ResourceSound();

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

    /*! Creates instance of sound object defined by resource. */
    PSound createInstance();

  private:

    ResourceSound(Application* app, ResourceGroup* group);

  private:

    /*! Name. */
    String m_name;
    /*! Pitch value. */
    float32 m_pitch;
    /* Sound data buffer. */
    PDataBuffer m_data;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCESOUND_H
