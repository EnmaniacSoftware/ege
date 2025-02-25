#ifndef EGE_CORE_SCENENODEOBJECT_H
#define EGE_CORE_SCENENODEOBJECT_H

#include "EGE.h"
#include "EGEMatrix.h"
#include "EGESignal.h"
#include "Core/Scene/SceneNode.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DECLARE_SMART_CLASS(SceneNodeObject, PSceneNodeObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SceneNodeObject : public Object
{
  friend class SceneNode;

  public:

    SceneNodeObject(const String& name, u32 uid = EGE_OBJECT_UID_GENERIC);
    virtual ~SceneNodeObject();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns object name. */
    const String& name() const { return m_name; }
    /*! Returns parent node. */
    SceneNode* parentNode() const{ return m_parentNode; }
    /*! Adds object render data for rendering with given renderer. */
    virtual bool addForRendering(IRenderer* renderer, const Matrix4f& transform = Matrix4f::IDENTITY);
    /*! Returns TRUE if object is visible. */
    bool isVisible() const { return m_visible; }
    /*! Sets visibility flag. */
    virtual void setVisible(bool set);
    /*! Returns TRUE if object is attached. */
    bool isAttached() const { return (NULL != m_parentNode); }

  protected:

    /*! Called when parent node changes. */
    virtual void parentChanged(SceneNode* oldNode, SceneNode* newNode);
    
    // bounding box related methods
   // inline const CAxisAlignedBox& getLocalAABB( void ) const { return m_cLocalAABB; } // gets local AABB

    // bounding sphere related methods
   // inline const CSphere& getLocalBS( void ) const { return m_cLocalBS; }             // gets local BS

    // ISceneNodeListener overrides
   // virtual void onSceneNodeAttached( SceneNode* pcNode );                           // notifies that object has been attached/detached
  //  virtual void onSceneNodeDestroyed( SceneNode* pcNode );                          // notifies that node has been destroyed

    // CRenderable overrides
  //  virtual void queryLights( void );                                                 // queries for lights affecing renderable

  private:

    /* Sets parent node. This should be called from SceneNode only. */
    void setParentNode(SceneNode* parent);

  private slots:

    /*! Slot called when parent node transformation has changed. */
    virtual void onParentNodeTransformationChanged();

  protected:

    /*! Name. */
    String m_name;

    //CAxisAlignedBox m_cLocalAABB;         // local AABB

    //CSphere m_cLocalBS;                   // local BS

    //SceneNode* m_pcNode;                 // pointer to node the object is attached to

    //// Comparator for light sorting based on squared distance from object
    //struct SLIGHTLESS
    //{
    //  _E3DExport bool operator()( const CLight* pcLight1, const CLight* pcLight2 ) const;
    //};

  private:

    /*! Pointer to parent node. */
    SceneNode* m_parentNode;
    /*! Visibility flag. */
    bool m_visible;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCENENODEOBJECT_H
