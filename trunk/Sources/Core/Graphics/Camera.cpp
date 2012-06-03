#include "Core/Graphics/Camera.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Components/Physics/PhysicsComponent.h"
#include "Core/Graphics/Viewport.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Camera)
EGE_DEFINE_DELETE_OPERATORS(Camera)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Camera::Camera(const String& name, SceneManager* sceneManager) : Frustum(sceneManager->app(), m_viewMatrix), 
                                                                 m_sceneManager(sceneManager), 
                                                                 m_name(name)
{
  // create new physics component
  m_physics = ege_new PhysicsComponent(sceneManager->app(), "camera");

  m_lookAt.set(0, 0, -1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Camera::~Camera()
{
  m_physics = NULL;
  m_sceneManager = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Sets look-at vector. */
void Camera::setLookAt(const Vector3f& point)
{
  m_lookAt = point;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Renders associated scene from camera's point of view onto given viewport. */
void Camera::render(PViewport viewport)
{
  if (m_sceneManager)
  {
    m_sceneManager->render(this, viewport);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns camera's view matrix. */
const Matrix4f& Camera::viewMatrix()
{
	// View matrix is:
	//
	//  [ Rx  Ux  Lx  Tx  ]
	//  [ Ry  Uy  Ly  Ty  ]
	//  [ Rz  Uz  Lz  Tz  ]
	//  [ 0   0   0   1   ]
	//
	// where:
  // 
  // R - right vector
  // U - up vector
  // L - look up vector
  // T - transposition vector = -(Transposed(Rot) * Pos)

  // TAGE - optimization issue
//  if (m_bViewMatrixNeedsUpdate)
  {

    // convert orientation into rotation matrix
    Quaternionf orientation = physics()->orientation();
    Math::Convert(&m_viewMatrix, &orientation);

    // calculate position (translation)
    Vector4f position = physics()->position();
    Matrix4f transposed = m_viewMatrix.transposed();
    Math::Transform(&position, &transposed);

    // store translation in modelview matrix
    m_viewMatrix.data[12] = -position.x;
    m_viewMatrix.data[13] = -position.y;
    m_viewMatrix.data[14] = -position.z;

    // TAGE testing
  //  m_viewMatrix = Matrix4f::IDENTITY;

    m_viewMatrixNeedsUpdate = false;
  }

  return m_viewMatrix;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Invalidates view matrix. */
void Camera::invalidateViewMatrix()
{
  m_viewMatrixNeedsUpdate = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid(). */
bool Camera::isValid() const
{
  return NULL != m_physics || !m_physics->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END