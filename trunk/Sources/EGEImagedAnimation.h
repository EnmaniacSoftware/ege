#ifndef EGE_IMAGEDANIMATION_H
#define EGE_IMAGEDANIMATION_H

#include "Core/Platform.h"
#include <EGEMatrix.h>
#include <EGEMaterial.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
namespace EGEImagedAnimation
{
  /*! Structure defining frame data info. */
  struct ActionData
  {
    s32 objectId;                             /*!< Object ID for which action is to be performed. */
  //  s32 queue;                                /*!< Render queue offset. */
    Matrix4f matrix;                          /*!< Transformation matrix. */
    Color color;                              /*!< Color to be used. */
  };

  /*! Child object data struct. */
  struct ChildObject
  {
    Matrix4f matrix;                          /*!< Transformation matrix. */
    Rectf rect;
    Vector2f size;

    PMaterial material;
  };

  typedef List<ChildObject> ChildObjectList;

  /*! Object data structure. */
  struct Object
  {
    s32 id;

    ChildObjectList children;
  };
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Core/Graphics/ImagedAnimation/ImagedAnimation.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGE_IMAGEDANIMATION_H