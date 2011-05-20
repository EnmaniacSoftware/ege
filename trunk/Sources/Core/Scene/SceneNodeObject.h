#ifndef EGE_CORE_SCENENODEOBJECT_H
#define EGE_CORE_SCENENODEOBJECT_H

#include "EGE.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Scene/SceneNode.h"
#include "EGEMemory.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DECLARE_SMART_CLASS(SceneNodeObject, PSceneNodeObject)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SceneNodeObject : public Object
{
  public:

    SceneNodeObject(SceneNode* pcParentNode, const EGEString& name);
    virtual ~SceneNodeObject();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Returns object name. */
    inline const EGEString& name() const { return m_name; }
    /*! Returns parent node. */
    inline SceneNode* parentNode() const{ return m_parentNode; }
    /* Adds object render data for rendering with given renderer. */
    bool addForRendering(Renderer* renderer) const;

    // bounding box related methods
   // inline const CAxisAlignedBox& getLocalAABB( void ) const { return m_cLocalAABB; } // gets local AABB

    // bounding sphere related methods
   // inline const CSphere& getLocalBS( void ) const { return m_cLocalBS; }             // gets local BS

    // ISceneNodeListener overrides
   // virtual void onSceneNodeAttached( SceneNode* pcNode );                           // notifies that object has been attached/detached
  //  virtual void onSceneNodeDestroyed( SceneNode* pcNode );                          // notifies that node has been destroyed

    // CRenderable overrides
  //  virtual void queryLights( void );                                                 // queries for lights affecing renderable

    bool addComponent(PComponent component);

    PRenderComponent renderComponent(const EGEString& name) const;
    
    PPhysicsComponent physicsComponent(const EGEString& name) const;

  protected:

    /*! Name. */
    EGEString m_name;
    /*! Pointer to parent node. */
    SceneNode* m_parentNode;

    std::vector<PComponent> m_components;     // components

    //CAxisAlignedBox m_cLocalAABB;         // local AABB

    //CSphere m_cLocalBS;                   // local BS

    //SceneNode* m_pcNode;                 // pointer to node the object is attached to

    //// Comparator for light sorting based on squared distance from object
    //struct SLIGHTLESS
    //{
    //  _E3DExport bool operator()( const CLight* pcLight1, const CLight* pcLight2 ) const;
    //};
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCENENODEOBJECT_H
