#ifndef EGE_CORE_SCENE_SCENEMANAGER_INTERFACE_H
#define EGE_CORE_SCENE_SCENEMANAGER_INTERFACE_H

#include "EGE.h"
#include "EGETime.h"
#include "Core/Engine/Interface/EngineModule.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class SceneNode;
EGE_DECLARE_SMART_CLASS(Camera, PCamera)
EGE_DECLARE_SMART_CLASS(Viewport, PViewport)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ISceneManager
{
  public:

    virtual ~ISceneManager() {}

  public:

    /*! Returns root node. 
     *  @return Root node. NULL if error.
     */
    virtual SceneNode* rootNode() const = 0;

    /*! Renders scene. 
     *  @param  camera    Camera from which point of view rendering is to be done.
     *  @param  viewport  Viewport for rendering.
     */
    virtual void render(PCamera camera, PViewport viewport) = 0;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCENE_SCENEMANAGER_INTERFACE_H