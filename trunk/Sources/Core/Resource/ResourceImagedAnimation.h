#ifndef EGE_CORE_RESOURCEIMAGEDANIMATION_H
#define EGE_CORE_RESOURCEIMAGEDANIMATION_H

/** Imaged animation resource definition class. This object (resource) contains definition of imaged animation object.
 *  Imaged animation resources define how set of images if being changed during the time line.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGETime.h>
#include <EGEDynamicArray.h>
#include <EGEList.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceImagedAnimation, PResourceImagedAnimation)
EGE_DECLARE_SMART_CLASS(ImagedAnimation, PImagedAnimation)
EGE_DECLARE_SMART_CLASS(ResourceMaterial, PResourceMaterial)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceImagedAnimation : public IResource
{
  public:

    virtual ~ResourceImagedAnimation();

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

    /* Creates instance of imaged animation object defined by resource. */
    PImagedAnimation createInstance();
    /* Set given instance of imaged animation object to what is defined by resource. */
    EGEResult setInstance(const PImagedAnimation& instance);

  private:

    ResourceImagedAnimation(Application* app, ResourceManager* manager);
    /* Returns TRUE if object is loaded. */
    bool isLoaded() const;
    /* Adds object. */
    EGEResult addObject(const PXmlElement& tag);
    /* Adds frame. */
    EGEResult addFrame(const PXmlElement& tag);

  private:

    /*! Object data. */
    struct ObjectData
    {
      String name;
      String materialName;
      Vector2f translate;
      Vector2f scale;
      Vector2f skew;

      PResourceMaterial materialResource;
    };

    /*! Frame data. */
    struct FrameData
    {
      String objectName;
      s32 queue;
      Vector2f translate;
      Vector2f scale;
      Vector2f skew;
    };

    typedef DynamicArray<ObjectData> ObjectDataArray;
    typedef List<FrameData> FrameDataList;

  private:

    /*! Name. */
    String m_name;
    /*! Frames per second. */
    s32 m_fps;
    /*! Objects array. */
    ObjectDataArray m_objects;
    /*! Frames list. */
    FrameDataList m_frames;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEIMAGEDANIMATION_H
