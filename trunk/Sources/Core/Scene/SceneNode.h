#ifndef EGE_CORE_SCENENODE_H
#define EGE_CORE_SCENENODE_H

// NOTE: scene manager should be passed by raw pointer to make sure it is deallocated properly in EGEngine
// NOTE: scene object should not be wrapped around smart pointer as it is shared with children and it would prevent 
//       correct deallocation

#include <EGE.h>
#include <EGETime.h>
#include <EGEList.h>
#include "Core/Data/Node.h"
#include "Core/Math/Matrix4.h"
#include "Core/Graphics/Camera.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class SceneManager;
EGE_DECLARE_SMART_CLASS(SceneNodeObject, PSceneNodeObject)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SceneNode : public Object, public Node //ListenerContainer<ISceneNodeListener>
{
  public:

    SceneNode(const String& name, SceneNode* parent, SceneManager* manager, EGEPhysics::ComponentType componentType = EGEPhysics::COMPONENT_DYNAMIC);
    virtual ~SceneNode();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Updates node. */
    void update(const Time& time);

    /*! Creates child scene node with a given name and attaches it. */
    SceneNode* createChildSceneNode(const String& name, EGEPhysics::ComponentType componentType = EGEPhysics::COMPONENT_DYNAMIC);

    /*! Attaches new object to node. */
    bool attachObject(PSceneNodeObject object);
    /*! Returns attached object with a given name. Returns NULL if no such object exists. */
    PSceneNodeObject attachedObject(const String& name) const;
    /*! Removes scene object of a given name. */
    void removeObject(const String& name);
    /*! Removes all objects. */
    void removeAllAttachedObjects();
   

    //typedef hash_map<string, SceneNodeObject*> AttachedObjectsMap;


    // scene manager related methods
    //SceneManager* getSceneManager( void ) const { return m_manager; }              // gets scene manager, node's creator

    //// attached object related methods
    //void                attachObject( SceneNodeObject* pcObject );                   // attaches given object to node
    //SceneNodeObject*   getAttachedObject( const string& strName ) const;             // gets given attached object, NULL if not found
    //AttachedObjectsMap& getAttachedObjects( void );                                   // gets attached objects
    //SceneNodeObject*   detachObject( const string& strName );                        // detaches given object from node
    //void                detachAllObjects( void );                                     // detaches all objects from node

    //// position related methods
    //inline const CVector3& getPosition( void ) const { return m_cPosition; }          // gets position
    //void                   setPosition( const CVector3& cPosition );                  // sets position
    //const CVector3&        getDerivedPosition( void );                                // gets derived position

    //// orientation related methods
    //inline const CQuaternion& getOrientation( void ) const { return m_cOrientation; } // gets orientation
    //void                      setOrientation( const CQuaternion& cOrientation );      // sets orientation
    //const CQuaternion&        getDerivedOrientation( void );                          // gets derived orientation

    //// bounding box related methods
    //inline const CAxisAlignedBox& getWorldAABB( void ) const { return m_cWorldAABB; } // gets world AABB

    //// bounding sphere related methods
    //inline const CSphere& getWorldBS( void ) const { return m_cWorldBS; }             // gets world BS

    //// matrix related methods

    /*! Finds visible objects from given camera point of view.
     *  @param  camera    Camera from which point of view rendering is being done.
     *  @param  renderer  Renderer accepting the object to show.
     *  @return TRUE if successful.
     */
    bool addForRendering(PCamera& camera, IRenderer* renderer) const;

  private:

    /*! Node override. Creates child node with a given name. MUST be overriden by subclass. */
    virtual Node* createChildNodeImpl(const String& name, EGEPhysics::ComponentType componentType) override;
    /*! Returns pointer to scene manager. */
    SceneManager* sceneManager() const { return m_manager; }

  private:

    /*! Raw pointer to owning manager. Pointer is RAW to avoid circular references (parent->child->parent). */
    SceneManager* m_manager;
    /*! List of node objects attached. */
    List<PSceneNodeObject> m_objects;

    //bool m_bChildrenNeedUpdate;                     // TRUE if child nodes needs to be updated
    //bool m_bNeedUpdate;                             // TRUE if update is needed
    //bool m_bTransformNeedUpdate;                    // TRUE if cached world transform needs to be updated

    //AttachedObjectsMap m_mpcObjects;                // attached objects

    //CVector3 m_cPosition;                           // node position
    //CVector3 m_cDerivedPosition;                    // node derived position (including parent's)

    //CQuaternion m_cOrientation;                     // node orientation
    //CQuaternion m_cDerivedOrientation;              // node derived orientation (including parent's)

    //CAxisAlignedBox m_cWorldAABB;                   // node world AABB

    //CSphere m_cWorldBS;                             // node world BS

    //CMatrix4 m_cWorldTransform;                     // cached world transform matrix

    //CWireBoundingBox* m_pcWireBoundingBox;          // wire bounding box, NULL if none
    //
    //CWireBoundingSphere* m_pcWireBoundingSphere;    // wire bounding sphere, NULL if none

    //typedef vector<SceneNode*> SceneNodesVector;
    //SceneNodesVector m_vpcChildNodes;               // child nodes


    //// update related methods
    //void updateBounds( void );                                                        // updates node bounds
    //void updateTransformation( void );                                                // updates transformation

    //// notification methods
    //void notifyVisible( void );                                                       // notifies listeners about self visibility
    //void notifyTransformChanged( void );                                              // notifies children nodes and attached object about our transformation change
    //void notifyDestroyed( void );                                                     // notifies attached objects that node is being destroyed
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCENENODE_H