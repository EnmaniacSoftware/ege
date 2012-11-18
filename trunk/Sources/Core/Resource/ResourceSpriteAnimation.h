#ifndef EGE_CORE_RESOURCESPRITEANIMATION_H
#define EGE_CORE_RESOURCESPRITEANIMATION_H

/** SpriteAnimation resource definition class. This object (resource) contains definition of sprite object.
 *  SpriteAnimation resources define how sequence of frames from sprite sheet should be animated (played).
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGETime.h>
#include <EGESpriteAnimation.h>
#include <EGEList.h>
#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceSpritesheet.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceSpriteAnimation, PResourceSpriteAnimation)
EGE_DECLARE_SMART_CLASS(ResourceSpritesheet, PResourceSpritesheet)
EGE_DECLARE_SMART_CLASS(SpriteAnimation, PSpriteAnimation)
EGE_DECLARE_SMART_CLASS(ResourceSequencer, PResourceSequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceSpriteAnimation : public IResource
{
  public:

    virtual ~ResourceSpriteAnimation();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Creates instance of resource. This method is a registration method for manager. */
    static PResource Create(Application* app, ResourceGroup* group);

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
    PSpriteAnimation createInstance();
    /* Set given instance of sprite object to what is defined by resource. */
    EGEResult setInstance(const PSpriteAnimation& instance);

  private:

    ResourceSpriteAnimation(Application* app, ResourceGroup* group);
    /*! Returns sprite sheet name. */
    inline const String& sheetName() const { return m_sheetName; } 
    /* Invalidates frame data. */
    void invalidateFrameData();
    /* Calculates frame data. */
    void calculateFrameData();
    /*! Returns spritesheet object containing sprite. */
    inline PResourceSpritesheet sheet() const { return m_sheet; }
    /* Adds sequence. */
    EGEResult addSequence(const PXmlElement& tag);

  private:

    typedef DynamicArray<EGESprite::FrameData> FameDataArray;
    typedef List<PResourceSequencer> SequenceResourceList;

  private:

    /*! Name. */
    String m_name;
    /*! SpriteAnimation sheet name. */
    String m_sheetName;
    /*! FPS playback count. */
    float32 m_fps;
    /*! Cached frame data validity flag. */
    bool m_frameDataInvalid;
    /*! Cached frame data. */
    FameDataArray m_frameData;
    /*! Loaded sheet. If NULL resource has not been loaded yet. */
    PResourceSpritesheet m_sheet;
    /*! List of sequence resources. */
    SequenceResourceList m_sequenceResources;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCESPRITEANIMATION_H
