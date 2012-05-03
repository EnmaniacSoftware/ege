#ifndef EGE_CORE_RESOURCESPRITE_H
#define EGE_CORE_RESOURCESPRITE_H

/** Sprite resource definition class. This object (resource) contains definition of sprite object.
 *  Sprite resources define how sequence of frames from sprite sheet should be animated (played).
 */

#include <EGE.h>
#include <EGEString.h>
#include <EGEXml.h>
#include <EGETime.h>
#include <EGESprite.h>
#include <EGEList.h>
#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceSpritesheet.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceManager;

EGE_DECLARE_SMART_CLASS(ResourceSprite, PResourceSprite)
EGE_DECLARE_SMART_CLASS(ResourceSpritesheet, PResourceSpritesheet)
EGE_DECLARE_SMART_CLASS(Sprite, PSprite)
EGE_DECLARE_SMART_CLASS(ResourceSequencer, PResourceSequencer)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceSprite : public IResource
{
  public:

    virtual ~ResourceSprite();

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
    PSprite createInstance();
    /* Set given instance of sprite object to what is defined by resource. */
    EGEResult setInstance(const PSprite& instance);

    /*! Returns number of frames. */
    //inline s32 frameCount() const { return m_frameCount; }
    ///*! Returns first frame index within sheet. */
    //inline s32 beginFrame() const { return m_beginFrame; }
    ///*! Returns ping-pong flag. */
    //inline bool pingPong() const { return m_pingPong; }
    ///*! Returns repeat flag. */
    //inline bool repeat() const { return m_repeat; }
    /*! Returns duration. */
   // inline const Time& duration() const { return m_duration; }
    /*! Returns repeat delay. */
    inline const Time& repeatDelay() const { return m_repeatDelay; }

  private:

    ResourceSprite(Application* app, ResourceManager* manager);
    /* Returns TRUE if object is loaded. */
    bool isLoaded() const;
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
    /*! Sprite sheet name. */
    String m_sheetName;
    /*! FPS playback count. */
    float32 m_fps;
    /*! Sprite play duration. */
   // Time m_duration;
    ///*! Ping-pong flag. If set sprite animation will progress forth and back. Mutually exclusive with 'repeat'. */
    //bool m_pingPong;
    ///*! Repeat flag. If set sprite animation will repeat once end is reached. Mutually exclusive with 'pingpong'. */
    //bool m_repeat;
    ///*! First frame index within sheet. */
    //s32 m_beginFrame;
    /*! Number of frames. */
  //  s32 m_frameCount;
    /*! Cached frame data validity flag. */
    bool m_frameDataInvalid;
    /*! Cached frame data. */
    FameDataArray m_frameData;
    /*! Loaded sheet. If NULL resource has not been loaded yet. */
    PResourceSpritesheet m_sheet;
    /*! Repeat delay. */
    Time m_repeatDelay;
    /*! List of sequence resources. */
    SequenceResourceList m_sequenceResources;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCESPRITE_H
