#ifndef EGE_CORE_CAMERA_H
#define EGE_CORE_CAMERA_H

#include "EGEString.h"
#include "EGEVector3.h"
#include "Core/Graphics/Frustum.h"
#include "Core/Component/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Engine;
class ISceneManager;

EGE_PREDECLARE_SMART_CLASS(Camera, PCamera)
EGE_PREDECLARE_SMART_CLASS(PhysicsComponent, PPhysicsComponent)
EGE_PREDECLARE_SMART_CLASS(Viewport, PViewport)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Camera : public Object
             , public Frustum
{
  public:

    Camera(Engine& engine, const String& name);
    virtual ~Camera();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Returns TRUE if object is valid(). */
    bool isValid() const;
    /*! Returns camera's name. */
    const String& name() const;
    /*! Returns camera's physics component. */
    PPhysicsComponent& physics();
    /*! Sets look-at vector. */
    void setLookAt(const Vector3f& point);
    /*! Returns look-at vector. */
    const Vector3f& getLookAt() const;
    /*! Returns camera's view matrix. */
    const Matrix4f& viewMatrix();
    /*! Renders associated scene from camera's point of view onto given viewport. */
    void render(PViewport viewport);

  protected:

    /*! Invalidates view matrix. */
    void invalidateViewMatrix();

  protected:

    /*! Position the camera is looking at (world coords). */
    Vector3f m_lookAt;
    /*! Scene manager camera is bound to. */
    ISceneManager* m_sceneManager;
    /*! TRUE if view matrix needs to be recalculated. */
    bool m_viewMatrixNeedsUpdate;
    /*! View matrix (model->world). */
    Matrix4f m_viewMatrix;
    /*! Local physics component. */
    PPhysicsComponent m_physics;

  private:

    /*! Name. */
    String m_name;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CAMERA_H