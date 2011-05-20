#ifndef EGE_CORE_SCENENODE_H
#define EGE_CORE_SCENENODE_H

// NOTE: scene manager should be passed by raw pointer to make sure it is deallocated properly in EGEngine
// NOTE: scene object should not be wrapped around smart pointer as it is shared with children and it would prevent 
//       correct deallocation

#include "EGE.h"
#include "Core/Graphics/Camera.h"
#include "Core/Timer/Time.h"
#include "Core/Math/Matrix4.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer;

EGE_DECLARE_SMART_CLASS(SceneNodeObject, PSceneNodeObject)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class SceneNode : public Object //ListenerContainer<ISceneNodeListener>
{
  public:

    SceneNode(const EGEString& name, SceneNode* pcParent, SceneManager* pcManager);
    virtual ~SceneNode();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns node name. */
    inline const EGEString& name() const { return m_name; }
    /*! Returns node's parent. NULL if this is root node. */
    inline SceneNode* parent() const { return m_parent; }
    /* Updates node. */
    void update(const Time& time);

    /* Creates child node with a given name and attaches it. */
    SceneNode* createChildNode(const EGEString& name);
    /* Deletes child node with a given name and detaches it. */
    void deleteChildNode(const EGEString& name);
    /* Deletes and detaches all child nodes. */
    void deleteAllChildNodes();
    /* Returns number of child nodes. */
    u32 childNodeCount() const;
    /* Returns child node with a given name. Returns NULL if no such node exists. */
    SceneNode* childNode(const EGEString& name) const;
    /* Returns child node with a given index. Returns NULL if no such node exists. */
    SceneNode* childNode(u32 index) const;

    /*! Returns local physics component. */
    inline PPhysicsComponent physics() { return m_physics; }
    /*! Returns cached combined world matrix. */
    inline const TMatrix4f& worldMatrix() const { return m_worldMatrix; }
    
    /* Creates and attaches new object to node. */
    PSceneNodeObject attachNewObject(const EGEString& name);
    /* Returns attached object with a given name. Returns NULL if no such object exists. */
    PSceneNodeObject attachedObject(const EGEString& name) const;
    /* Removes scene object of a given name. */
    void removeObject(const EGEString& name);
    /* Removes all objects. */
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

    //// visbility related methods
    //bool isVisible( void ) const { return m_bVisible; }                               // returns TRUE if node is visible
    //void setVisible( bool bEnable );                                                  // shows/hides the node (from rendering/processing point of view)

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

    bool addForRendering(PCamera& pCamera, Renderer* pcRenderer) const;                              // finds visible objects from given camera point of view, returns TRUE if successful

  protected:

    /*! Name. */
    EGEString m_name;
    /*! Raw pointer to owning manager. Pointer is RAW to avoid circular references (parent->child->parent). */
    SceneManager* m_manager;
    /*! Raw pointer to parent node. NULL if this is root node. Pointer is RAW as this object is not intended (TAGE for now ?) to be shared. */
    SceneNode* m_parent;
    /*! List of node objects attached. */
    std::vector<PSceneNodeObject> m_objects;
    /*! List of all child nodes attached. */
    std::vector<SceneNode*> m_children;
    /*! Physics component. */
    PPhysicsComponent m_physics;
    /*! Cached combined world matrix from all self and all parent nodes. */
    TMatrix4f m_worldMatrix;

    //bool m_bChildrenNeedUpdate;                     // TRUE if child nodes needs to be updated
    //bool m_bVisible;                                // TRUE if node is visible
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