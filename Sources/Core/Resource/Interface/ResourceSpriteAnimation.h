#ifndef EGE_CORE_RESOURCESPRITEANIMATION_H
#define EGE_CORE_RESOURCESPRITEANIMATION_H

/** SpriteAnimation resource definition class. This object (resource) contains definition of sprite object.
 *  SpriteAnimation resources define how sequence of frames from sprite sheet should be animated (played).
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGETime.h"
#include "EGESpriteAnimation.h"
#include "EGEList.h"
#include "Core/Resource/Interface/Resource.h"
#include "Core/Resource/Interface/ResourceSpritesheet.h"

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

    /*! Creates instance of sprite object defined by resource. */
    PSpriteAnimation createInstance();
    /*! Set given instance of sprite object to what is defined by resource. */
    EGEResult setInstance(const PSpriteAnimation& instance);

  private:

    ResourceSpriteAnimation(Engine& engine, ResourceGroup* group);
    /*! Returns sprite sheet name. */
    const String& sheetName() const { return m_sheetName; } 
    /*! Invalidates frame data. */
    void invalidateFrameData();
    /*! Calculates frame data. */
    void calculateFrameData();
    /*! Returns spritesheet object containing sprite. */
     PResourceSpritesheet sheet() const { return m_sheet; }
    /*! Adds sequence. */
    EGEResult addSequence(const PXmlElement& tag);

  private:

    typedef DynamicArray<SpriteAnimationFrameData> FameDataArray;
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
