#ifndef EGE_CORE_FRUSTUM_H
#define EGE_CORE_FRUSTUM_H

// This class is intended to be a part of Camera class rather then stand alone instance

#include "EGE.h"
#include "Core/Math/Matrix4.h"
#include "Core/Math/Plane.h"
#include "Core/Math/angle.h"
#include "Core/Math/Rect.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Frustum : public Object
{
  public:

    Frustum(Application* app, const TMatrix4f& viewMatrix);
    virtual ~Frustum();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    enum ProjectionType
    {
      ORTHOGRAPHIC,
      PERSPECTIVE
    };

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

    /* Updates frustum. */
    void update();
    /*! Returns projection type. */
    inline ProjectionType projectionType() const { return m_projectionType; }
    /* Sets projection type. */
    void setProjectionType(ProjectionType projection);
    /*! Returns near plane distance. */
    inline float32 nearPlaneDistance() const { return m_nearPlaneDistance; }
    /* Sets near plane distance. */
    void setNearPlaneDistance(float32 distance);
    /*! Returns far plane distance. */
    inline float32 farPlaneDistance() const { return m_farPlaneDistance; }
    /* Sets far plane distance. */
    void setFarPlaneDistance(float32 distance);
    /*! Returns aspect ratio. */
    inline float32 aspectRatio() const { return m_aspectRatio; }
    /* Sets aspect ratio. */
    void setAspectRatio(float32 aspectRatio);
    /*! Returns Field-Of-View along Y-axis. */
    inline const Angle& FOVy() const { return m_FOVy; }
    /* Sets Field-Of-View. */
    void setFOVy(const Angle& angle);
    /* Returns projection matrix. */
    const TMatrix4f& projectionMatrix();
    /* Sets orthographic view settings. */
    void setOrthoSettings(const TRectf& rect);
//    bool isVisible( const CAxisAlignedBox& cBox );                                   // returns TRUE if given AABB is visible
//    bool isVisible( const CSphere& cSphere );                                        // returns TRUE if given sphere is visible
    /* Returns TRUE if point is within vfrustum. */
    bool isVisible(const TVector4f& point);

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
    TPlanef m_planes[PLANE_COUNT];
    /*! Projection matrix. */
    TMatrix4f m_projectionMatrix;
    /*! TRUE if projection matrix needs to be updated. */
    bool m_projectionMatrixNeedsUpdate;
    /*! TRUE if frustum planes need update. */
    bool m_planesNeedUpdate;
    /*! Orthographic projection settings. */
    TRectf m_orthoRect;

    //bool m_bUpdateView;                     // TRUE if view has to be updated
    //bool m_bUpdateFrustumPlanes;            // TRUE if frustum planes need to be updated

  private:

    /*! Reference to camera's view matrix. */
    const TMatrix4f& m_viewMatrix;

  private:

    /* Invalidates frustum planes. */
    void invalidateFrustumPlanes();
    /* Calculates frustum planes. */
    void calculateFrustumPlanes();
    /* Invalidates projection matrix. */
    void invalidateProjectionMatrix();
    /* Calculates projetction matrix. */
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