#include "Core/Graphics/Frustum.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(Frustum)
EGE_DEFINE_DELETE_OPERATORS(Frustum)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Frustum::Frustum(Application* app, const Matrix4f& viewMatrix) : Object(app), 
                                                                 m_projectionType(Frustum::PERSPECTIVE), 
                                                                 m_nearPlaneDistance(1.0f), 
                                                                 m_farPlaneDistance(100.0f), 
                                                                 m_aspectRatio(1.33f), 
                                                                 m_projectionMatrixNeedsUpdate(true), 
                                                                 m_planesNeedUpdate(true), 
                                                                 m_viewMatrix(viewMatrix)
{
  m_FOVy.fromDegrees(45.0f);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Frustum::~Frustum()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::setProjectionType(ProjectionType projection)
{
  m_projectionType = projection;

  // invalidate frustum
  invalidateProjectionMatrix();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::setNearPlaneDistance(float32 distance)
{
  m_nearPlaneDistance = distance;

  // invalidate frustum
  invalidateProjectionMatrix();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::setFarPlaneDistance(float32 distance)
{
  m_farPlaneDistance = distance;

  // invalidate frustum
  invalidateProjectionMatrix();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::setAspectRatio(float32 aspectRatio)
{
  m_aspectRatio = aspectRatio;

  // invalidate frustum
  invalidateProjectionMatrix();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::setFOVy(const Angle& angle)
{
  m_FOVy = angle;

  // invalidate frustum
  invalidateProjectionMatrix();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::update()
{
  calculateProjectionMatrix();
  calculateFrustumPlanes();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//bool Frustum::isVisible( const CAxisAlignedBox& cBox )
//{
//  // check if NULL box
//  if ( cBox.isNull() == true )
//  {
//    // always invisible
//    return false;
//  }
//
//  // check if INFINITE box
//  if ( cBox.isInfinite() == true )
//  {
//    // always visible
//    return true;
//  }
//
//  // update all
//  update();
//
//  // get centre of the box
//  CVector3 cCentre = cBox.getCenter();
//
//  // get the half-size of the box
//  CVector3 cHalfSize = cBox.getHalfSize();
//
//  // For each plane, see if all points are on the negative side
//  // If so, object is not visible
//  for ( u32 uiPlane = 0; uiPlane < PLANE_COUNT; ++uiPlane )
//  {
//    // Skip far plane if infinite view frustum
//    //if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
//    //    continue;
//
//    CPlane::ESide eSide = m_cPlanes[ uiPlane ].getSide( &cCentre, &cHalfSize );
//
//    if ( eSide == CPlane::NEGATIVE_SIDE )
//    {
//      // ALL corners on negative side therefore out of view
//      //if (culledBy)
//      //    *culledBy = (FrustumPlane)plane;
//      return false;
//    }
//  }
//
//  return true;
//}
//
//bool Frustum::isVisible( const CSphere& cSphere )
//{
//  // check if NULL sphere
//  if ( cSphere.isNull() == true )
//  {
//    // always invisible
//    return false;
//  }
//
//  // update all
//  update();
//
//  // For each plane, see if sphere is on negative side
//  // If so, object is not visible
//  for ( u32 uiPlane = 0; uiPlane < PLANE_COUNT; ++uiPlane )
//  {
//    // Skip far plane if infinite view frustum
//    //if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
//    //    continue;
//
//    // If the distance from sphere center to plane is negative, and 'more negative' 
//    // than the radius of the sphere, sphere is outside frustum
//    if ( m_cPlanes[ uiPlane ].getDistance( cSphere.getCenter() ) < -cSphere.getRadius() )
//    {
//      // ALL corners on negative side therefore out of view
//      //if (culledBy)
//      //    *culledBy = (FrustumPlane)plane;
//      return false;
//    }
//  }
//
//  return true;
//}
//
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Frustum::isVisible(const Vector3f& point)
{
  // make sure all is up to date
  update();

  // for each plane, see if all points are on the negative side. If so, object is not visible
  for (u32 plane = 0; plane < PLANE_COUNT; ++plane)
  {
    // Skip far plane if infinite view frustum
    //if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
    //    continue;

    if (ENegative == m_planes[plane].side(point))
    {
      // ALL corners on negative side therefore out of view
      //if (culledBy)
      //    *culledBy = (FrustumPlane)plane;
      return false;
    }
  }

  return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const Matrix4f& Frustum::projectionMatrix()
{ 
  calculateProjectionMatrix();

  return m_projectionMatrix; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::calculateProjectionMatrix()
{
  // check if update is needed
  if (m_projectionMatrixNeedsUpdate)
  {
    // check if PERSPECTIVE projection is used
    if (Frustum::PERSPECTIVE == m_projectionType)
    {
      // create projection matrix

      // Projection matrix is of the following format (OGL compilant):
      //
      // f/a     0      0      0
      //  0      f      0      0
      //  0      0      A      B
      //  0      0     -1      0
      //
      // where:
      // 
      // a = aspect ratio
      // f = cotangent(fovy / 2)
      // A = (zfar + znear) / (znear - zfar)
      // B = (2 * zfar * znear) / (znear - zfar)

      float32 f                = Math::Ctg(m_FOVy.radians() * 0.5f);
      float32 fNearMinusFarRep = 1.0f /(nearPlaneDistance() - farPlaneDistance());

      m_projectionMatrix.data[0]  = f / aspectRatio();
      m_projectionMatrix.data[1]  = 0;
      m_projectionMatrix.data[2]  = 0;
      m_projectionMatrix.data[3]  = 0;
      m_projectionMatrix.data[4]  = 0;
      m_projectionMatrix.data[5]  = f;
      m_projectionMatrix.data[6]  = 0;
      m_projectionMatrix.data[7]  = 0;
      m_projectionMatrix.data[8]  = 0;
      m_projectionMatrix.data[9]  = 0;
      m_projectionMatrix.data[10] = (farPlaneDistance() + nearPlaneDistance()) * fNearMinusFarRep;
      m_projectionMatrix.data[11] = -1;
      m_projectionMatrix.data[12] = 0;
      m_projectionMatrix.data[13] = 0;
      m_projectionMatrix.data[14] = 2.0f * farPlaneDistance() * nearPlaneDistance() * fNearMinusFarRep;
      m_projectionMatrix.data[15] = 0;
    }
    else
    {
      // create projection matrix (ORHTHOGONAL)

      // Projection matrix is of the following format:
      //
      //  A     0     0      Tx
      //  0     B     0      Ty
      //  0     0     C      Tz
      //  0     0     0     -1
      //
      // where:
      // 
      // A = 2 / (right - left)
      // B = 2 / (top - bottom)         // swap this to have native OpenGL Y axis
      // C = -2 / (far - near)
      // Tx = -(right + left) / (right - left)
      // Ty = -(top + bottom) / (top - bottom)
      // Tz = -(far + near) / (far - near)
     
      float32 invWidth    = 1.0f / ((m_orthoRect.right() + 1) - m_orthoRect.left());
      float32 invHeight   = 1.0f / (m_orthoRect.top() - (m_orthoRect.bottom() + 1));
      float32 invDistance = 1.0f / (farPlaneDistance() - nearPlaneDistance());

      m_projectionMatrix.data[0]  = 2.0f * invWidth;
      m_projectionMatrix.data[1]  = 0;
      m_projectionMatrix.data[2]  = 0;
      m_projectionMatrix.data[3]  = 0;
      m_projectionMatrix.data[4]  = 0;
      m_projectionMatrix.data[5]  = 2.0f * invHeight;
      m_projectionMatrix.data[6]  = 0;
      m_projectionMatrix.data[7]  = 0;
      m_projectionMatrix.data[8]  = 0;
      m_projectionMatrix.data[9]  = 0;
      m_projectionMatrix.data[10] = - 2.0f * invDistance;
      m_projectionMatrix.data[11] = 0;
      m_projectionMatrix.data[12] = - ((m_orthoRect.right() + 1) + m_orthoRect.left()) * invWidth;
      m_projectionMatrix.data[13] = - (m_orthoRect.top() + (m_orthoRect.bottom() + 1)) * invHeight;
      m_projectionMatrix.data[14] = - (farPlaneDistance() + nearPlaneDistance()) * invDistance;
      m_projectionMatrix.data[15] = 1;
    }

    // invalidate frustum planes
    invalidateFrustumPlanes();

    // reset flag
    m_projectionMatrixNeedsUpdate = false;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::calculateFrustumPlanes()
{
  // check if update is necessary
  if (m_planesNeedUpdate)
  {
    Matrix4f clipping;
    Vector3f normal;

	  // calculate clipping planes ( P*V )
    clipping = m_projectionMatrix.multiply(m_viewMatrix);

	  // calculate RIGHT plane
    normal.x = clipping.data[3] - clipping.data[0];
    normal.y = clipping.data[7] - clipping.data[4];
    normal.z = clipping.data[11] - clipping.data[8];

    m_planes[PLANE_RIGHT].create(normal, clipping.data[1 ] - clipping.data[12]);
	  m_planes[PLANE_RIGHT].normalize();

	  // calculate LEFT plane
    normal.x = clipping.data[3] + clipping.data[0];
    normal.y = clipping.data[7] + clipping.data[4];
    normal.z = clipping.data[11] + clipping.data[8];

    m_planes[PLANE_LEFT].create(normal, clipping.data[15] + clipping.data[12]);
	  m_planes[PLANE_LEFT].normalize();

	  // calculate TOP plane
    normal.x = clipping.data[3] - clipping.data[1];
    normal.y = clipping.data[7] - clipping.data[5];
    normal.z = clipping.data[11] - clipping.data[9];

    m_planes[PLANE_TOP].create(normal, clipping.data[15] - clipping.data[13]);
	  m_planes[PLANE_TOP].normalize();

    // calculate BOTTOM plane
    normal.x = clipping.data[3] + clipping.data[1];
    normal.y = clipping.data[7] + clipping.data[5];
    normal.z = clipping.data[11] + clipping.data[9];

    m_planes[PLANE_BOTTOM].create(normal, clipping.data[15] + clipping.data[13]);
	  m_planes[PLANE_BOTTOM].normalize();

	  // calculate FAR plane
    normal.x = clipping.data[3] - clipping.data[2];
    normal.y = clipping.data[7] - clipping.data[6];
    normal.z = clipping.data[11] - clipping.data[10];

    m_planes[PLANE_FAR].create(normal, clipping.data[15] - clipping.data[14]);
	  m_planes[PLANE_FAR].normalize();

    // calculate NEAR plane
    normal.x = clipping.data[3] + clipping.data[2];
    normal.y = clipping.data[7] + clipping.data[6];
    normal.z = clipping.data[11] + clipping.data[10];

    m_planes[PLANE_NEAR].create(normal, clipping.data[15] + clipping.data[14]);
	  m_planes[PLANE_NEAR].normalize();

    // reset flag
    m_planesNeedUpdate = false;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::invalidateProjectionMatrix()
{
  m_projectionMatrixNeedsUpdate = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::invalidateFrustumPlanes()
{
  m_planesNeedUpdate = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frustum::setOrthoSettings(const Rectf& rect)
{
  // store settings
  if (rect != m_orthoRect)
  {
    m_orthoRect = rect;

    if (projectionType() == Frustum::ORTHOGRAPHIC)
    {
      invalidateProjectionMatrix();
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//void Frustum::updateFrustumPlanes( void )
//{
//  // check if planes need update
//  if ( m_bUpdateFrustumPlanes == true )
//  {
//    // calculate frustum planes
//    calculateFrustumPlanes();
//
//    // reset flag
//    m_bUpdateFrustumPlanes = false;
//  }
//}
//
//bool Frustum::project( const CVector3& cPosition, CVector3& cDeviceCoords )
//{
//  // update view
//  updateView();
//
//  // transform from WORLD space into camera EYE space
//  CVector3 cEyeSpacePos = m_cViewMatrix.transformAffine( &cPosition );
//
//  // check if in front of frustum
//  if ( cEyeSpaePos.z < 0 )
//  {
//    // update frustum
//    updateFrustum();
//
//    // convert into device coordinates
//    cDeviceCoords = m_cProjectionMatrix.transform( &cEyeSpacePos );
//
//    // in front of frustum
//    return true;
//  }
//
//  // not visible, behind frustum
//  return false;
//}
//
//void Frustum::unProject( const CVector3& cWindowPosition, const Viewport* pcViewport, CVector3& cPosition )
//{
//  // update view
//  updateView();
//
//	// calculate inverse of P*V
//  CMatrix4 cPVInv = m_cProjectionMatrix.concatenate( m_cViewMatrix ).getInverse();
//
//  // calculate normalized window coordinates and transform it by matrix
//  cPosition = cPVInv.transform( &CVector3( 2*( cWindowPosition.x-pcViewport->getActualRect().x )/pcViewport->getActualRect().width-1,
//                                           2*( cWindowPosition.y-pcViewport->getActualRect().y )/pcViewport->getActualRect().height-1,
//                                           2*cWindowPosition.z-1 ) );
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// PRIVATES

EGE_NAMESPACE_END