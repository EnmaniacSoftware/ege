#ifndef EGE_CORE_FRUSTUM_H
#define EGE_CORE_FRUSTUM_H

/** This class is intended to be a part of Camera class rather then stand alone instance.
 */

#include "EGE.h"
#include "EGEMatrix.h"
#include "EGERect.h"
#include "EGEPlane.h"
#include "EGEAngle.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Frustum : public Object
{
  public:

    Frustum(Application* app, const Matrix4f& viewMatrix);
    virtual ~Frustum();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

  public:

    /*! Available projection types. */
    enum ProjectionType
    {
      ORTHOGRAPHIC,
      PERSPECTIVE
    };

    /*! Available planes. */
    enum Plane
    {
      PLANE_NEAR = 0,
      PLANE_FAR,
      PLANE_LEFT,
      PLANE_RIGHT,
      PLANE_TOP,
      PLANE_BOTTOM,
      PLANE_COUNT
    };

  public:

    /*! Updates frustum. */
    void update();
    /*! Returns projection type. */
    ProjectionType projectionType() const { return m_projectionType; }
    /*! Sets projection type. */
    void setProjectionType(ProjectionType projection);
    /*! Returns near plane distance. */
    float32 nearPlaneDistance() const { return m_nearPlaneDistance; }
    /*! Sets near plane distance. */
    void setNearPlaneDistance(float32 distance);
    /*! Returns far plane distance. */
    float32 farPlaneDistance() const { return m_farPlaneDistance; }
    /*! Sets far plane distance. */
    void setFarPlaneDistance(float32 distance);
    /*! Returns aspect ratio. */
    float32 aspectRatio() const { return m_aspectRatio; }
    /*! Sets aspect ratio. */
    void setAspectRatio(float32 aspectRatio);
    /*! Returns Field-Of-View along Y-axis. */
    const Angle& FOVy() const { return m_FOVy; }
    /*! Sets Field-Of-View. */
    void setFOVy(const Angle& angle);
    /*! Returns projection matrix. */
    const Matrix4f& projectionMatrix();
    /*! Sets orthographic view settings. */
    void setOrthoSettings(const Rectf& rect);
//    bool isVisible( const CAxisAlignedBox& cBox );                                   // returns TRUE if given AABB is visible
//    bool isVisible( const CSphere& cSphere );                                        // returns TRUE if given sphere is visible
    /*! Returns TRUE if point is within vfrustum. */
    bool isVisible(const Vector3f& point);

    // projection related methods
    //virtual bool project( const CVector3& cPosition, CVector3& cDeviceCoords );      // projects world coordinates into device coordinates, 
    //                                                                                 // returns TRUE if in front of frustum
    //virtual void unProject( const CVector3& cWindowPosition, 
    //                        const Viewport* pcViewport, CVector3& cPosition );      // maps window to world coordinates

  protected:

    /*! Projection type. */
    ProjectionType m_projectionType;
    /*! Near plane distance. Shared between projections. */
    float32 m_nearPlaneDistance;
    /*! Far plane distance. Shared between projections. */
    float32 m_farPlaneDistance;
    /*! Aspect ration. */
    float32 m_aspectRatio;
    /*! Field-Of-View along Y-axis. */
    Angle m_FOVy;
    /*! Frustum planes. */
    Planef m_planes[PLANE_COUNT];
    /*! Projection matrix. */
    Matrix4f m_projectionMatrix;
    /*! TRUE if projection matrix needs to be updated. */
    bool m_projectionMatrixNeedsUpdate;
    /*! TRUE if frustum planes need update. */
    bool m_planesNeedUpdate;
    /*! Orthographic projection settings. */
    Rectf m_orthoRect;

    //bool m_bUpdateView;                     // TRUE if view has to be updated
    //bool m_bUpdateFrustumPlanes;            // TRUE if frustum planes need to be updated

  private:

    /*! Reference to camera's view matrix. */
    const Matrix4f& m_viewMatrix;

  private:

    /*! Invalidates frustum planes. */
    void invalidateFrustumPlanes();
    /*! Calculates frustum planes. */
    void calculateFrustumPlanes();
    /*! Invalidates projection matrix. */
    void invalidateProjectionMatrix();
    /*! Calculates projetction matrix. */
    void calculateProjectionMatrix();
    //void invalidateFrustumPlanes( void );                                                     // invalidates frustum planes
    //void updateFrustumPlanes( void );                                                         // updates frustum planes
    //
    //// model view related methods
    //virtual void calculateViewMatrix( void );                                                 // calculates view matrix

    //// view related methods
    //virtual void invalidateView( void );                                                      // invalidates view
    //virtual void updateView( void );                                                          // updates view
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_FRUSTUM_H