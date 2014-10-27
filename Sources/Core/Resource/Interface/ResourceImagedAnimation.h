#ifndef EGE_CORE_RESOURCEIMAGEDANIMATION_H
#define EGE_CORE_RESOURCEIMAGEDANIMATION_H

/** Imaged animation resource definition class. This object (resource) contains definition of imaged animation object.
 *  Imaged animation resources define how set of images if being changed during the time line.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "EGETime.h"
#include "EGEDynamicArray.h"
#include "EGEList.h"
#include "EGERect.h"
#include "EGEImagedAnimation.h"
#include "Core/Resource/Interface/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;
EGE_DECLARE_SMART_CLASS(ResourceImagedAnimation, PResourceImagedAnimation)
EGE_DECLARE_SMART_CLASS(ImagedAnimation, PImagedAnimation)
EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)
EGE_DECLARE_SMART_CLASS(ResourceSequencer, PResourceSequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceImagedAnimation : public IResource
{
  struct FrameData;

  public:

    ResourceImagedAnimation(Engine& engine, ResourceGroup* group);
    virtual ~ResourceImagedAnimation();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! @see IResource::name. */
    const String& name() const override;
    /*! @see IResource::create. */ 
    EGEResult create(const String& path, const PObject& data) override;
    /*! @see IResource::load. */
    EGEResult load() override;
    /*! @see IResource::unload. */
    void unload() override;

    /*! Creates instance of imaged animation object defined by resource. */
    PImagedAnimation createInstance();
    /*! Set given instance of imaged animation object to what is defined by resource. */
    EGEResult setInstance(const PImagedAnimation& instance);

  private:

    /*! Adds object. */
    EGEResult addObject(const PXmlElement& tag);
    /*! Adds frame. */
    EGEResult addFrame(const PXmlElement& tag);
    /*! Adds action to the given frame. */
    EGEResult addAction(const PXmlElement& tag, FrameData* frameData) const;
    /*! Adds sequence. */
    EGEResult addSequence(const PXmlElement& tag);

  private:
    
    /*! Object child data struct. */
    struct ObjectChildData
    {
      String materialName;
      Vector2f translate;
      Vector2f scale;
      Vector2f skew;
      Rectf rect;
      Vector2f size;

      PResourceMaterial materialResource;
    };

    typedef List<ObjectChildData> ObjectChildDataList;

    /*! Object data structure. */
    struct ObjectData
    {
      s32 id;

      ObjectChildDataList children;
    };

    /*! Frame action data struct. */
    struct FrameActionData
    {
      s32 objectId;
   //   s32 queue;
      Vector2f translate;
      Vector2f scale;
      Vector2f skew;
      Color color;
    };

    /*! Frame data structure. */
    struct FrameData
    {
      List<FrameActionData> actions;
    };

    typedef DynamicArray<ObjectData> ObjectDataArray;
    typedef List<FrameData> FrameDataList;
    typedef List<FrameActionData> FrameActionDataList;
    typedef List<PResourceSequencer> SequenceResourceList;

  private:

    /*! Name. */
    String m_name;
    /*! FPS playback count. */
    float32 m_fps;
    /*! Objects array. */
    ObjectDataArray m_objects;
    /*! Frames list. */
    FrameDataList m_frames;
    /*! Display size (in pixels). */
    Vector2f m_displaySize;
    /*! List of sequence resources. */
    SequenceResourceList m_sequenceResources;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEIMAGEDANIMATION_H
