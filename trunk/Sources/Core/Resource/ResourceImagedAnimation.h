#ifndef EGE_CORE_RESOURCEIMAGEDANIMATION_H
#define EGE_CORE_RESOURCEIMAGEDANIMATION_H

/** Imaged animation resource definition class. This object (resource) contains definition of imaged animation object.
 *  Imaged animation resources define how set of images if being changed during the time line.
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGETime.h>
#include "Core/Resource/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceImagedAnimation, PResourceImagedAnimation)
EGE_DECLARE_SMART_CLASS(ImagedAnimation, PImagedAnimation)

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

    /*! Returns number of frames. */
    //inline s32 frameCount() const { return m_frameCount; }
    ///*! Returns first frame index within sheet. */
    //inline s32 beginFrame() const { return m_beginFrame; }
    ///*! Returns ping-pong flag. */
    //inline bool pingPong() const { return m_pingPong; }
    ///*! Returns repeat flag. */
    //inline bool repeat() const { return m_repeat; }
    ///*! Returns duration. */
    //inline const Time& duration() const { return m_duration; }

  private:

    ResourceImagedAnimation(Application* app, ResourceManager* manager);
    /* Returns TRUE if object is loaded. */
    inline bool isLoaded() const { return false; }//NULL != m_sheet; }
    /*! Returns sprite sheet name. */
    //inline const String& sheetName() const { return m_sheetName; } 
    ///* Invalidates frame data. */
    //void invalidateFrameData();
    ///* Calculates frame data. */
    //void calculateFrameData();
    ///*! Returns spritesheet object containing sprite. */
    //inline PResourceSpritesheet sheet() const { return m_sheet; }

  private:

//    typedef DynamicArray<EGESprite::FrameData> FameDataArray;

  private:

    /*! Name. */
    String m_name;
    ///*! Sprite sheet name. */
    //String m_sheetName;
    ///*! Sprite play duration. */
    //Time m_duration;
    ///*! Ping-pong flag. If set sprite animation will progress forth and back. Mutually exclusive with 'repeat'. */
    //bool m_pingPong;
    ///*! Repeat flag. If set sprite animation will repeat once end is reached. Mutually exclusive with 'pingpong'. */
    //bool m_repeat;
    ///*! First frame index within sheet. */
    //s32 m_beginFrame;
    ///*! Number of frames. */
    //s32 m_frameCount;
    ///*! Cached frame data validity flag. */
    //bool m_frameDataInvalid;
    ///*! Cached frame data. */
    //FameDataArray m_frameData;
    ///*! Loaded sheet. If NULL resource has not been loaded yet. */
    //PResourceSpritesheet m_sheet;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCEIMAGEDANIMATION_H
