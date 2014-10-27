#ifndef EGE_CORE_RESOURCESPRITESHEET_H
#define EGE_CORE_RESOURCESPRITESHEET_H

/** SpriteAnimation sheet resource definition class. This object (resource) contains definition of sprite sheet object.
 *  SpriteAnimation sheets describes series of frames which can be played one after another to form sort of animation. Frames are counted from left to right and from 
 *  first (top) row to last (bottom) one.
 */

#include "EGE.h"
#include "EGEString.h"
#include "EGEXml.h"
#include "Core/Resource/Interface/Resource.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceGroup;

EGE_DECLARE_SMART_CLASS(ResourceSpritesheet, PResourceSpritesheet)
EGE_DECLARE_SMART_CLASS(TextureImage, PTextureImage)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ResourceSpritesheet : public IResource
{
  public:

    ResourceSpritesheet(Engine& engine, ResourceGroup* group);
    virtual ~ResourceSpritesheet();

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

    /*! Returns number of frames. */
    s32 frameCount() const { return m_frameCount; }
    /*! Returns frame width (in texels). */
    s32 frameWidth() const { return m_frameWidth; }
    /*! Returns frame height (in texels). */
    s32 frameHeight() const { return m_frameHeight; }
    /*! Returns number of frames in a row. */
    s32 framesPerRow() const { return m_framesPerRow; }
    /*! Returns texture image containing sheet pixel data. */
    PTextureImage textureImage() const { return m_texture; }

  private:

    /*! Returns texture name. */
    const String& textureName() const { return m_textureName; } 

  private:

    /*! Name. */
    String m_name;
    /*! Texture name. */
    String m_textureName;
    /*! Frame width (in texels). */
    s32 m_frameWidth;
    /*! Frame height (in texels). */
    s32 m_frameHeight;
    /*! Number of frames. */
    s32 m_frameCount;
    /*! Number of frames in a single row. */
    s32 m_framesPerRow;
    /*! Loaded texture. If NULL resource has not been loaded yet. */
    PTextureImage m_texture;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_RESOURCESPRITESHEET_H
