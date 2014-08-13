#include "Core/Graphics/Camera.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Physics/PhysicsManager.h"
#include "Core/Component/Physics/PhysicsComponent.h"
#include "Core/Graphics/Viewport.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Camera)
EGE_DEFINE_DELETE_OPERATORS(Camera)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Camera::Camera(const String& name, SceneManager* sceneManager) : Frustum(m_viewMatrix)
                                                               , m_sceneManager(sceneManager)
                                                               , m_name(name)
{
  // create new physics component
  m_physics = ege_new PhysicsComponent(sceneManager->engine(), "camera");

  m_lookAt.set(0, 0, -1);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Camera::~Camera()
{
  m_physics = NULL;
  m_sceneManager = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Camera::setLookAt(const Vector3f& point)
{
  m_lookAt = point;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Camera::render(PViewport viewport)
{
  if (m_sceneManager)
  {
    m_sceneManager->render(this, viewport);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    Math::Convert(m_viewMatrix, orientation);

    // calculate position (translation)
    Vector4f position = physics()->position();
    Matrix4f transposed = m_viewMatrix.transposed();
    position = Math::Transform(position, transposed);

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
void Camera::invalidateViewMatrix()
{
  m_viewMatrixNeedsUpdate = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Camera::isValid() const
{
  return NULL != m_physics || !m_physics->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END