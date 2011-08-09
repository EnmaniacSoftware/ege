#ifndef EGE_CORE_NODE_H
#define EGE_CORE_NODE_H

/** Class representing a general-purpose node an articulated scene graph.
    @remarks
        A node in the scene graph is a node in a structured tree. A node contains
        information about the transformation which will apply to
        it and all of it's children. Child nodes can have transforms of their own, which
        are combined with their parent's transformations.
    @par
        This is an abstract class - concrete classes are based on this for specific purposes,
        e.g. SceneNode, Bone
*/

#include <EGE.h>
#include <EGEList.h>
#include "Core/Math/Matrix4.h"
#include "Core/Components/Physics/PhysicsComponent.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Renderer;
class Application;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Node
{
  public:

    Node(Application* app, const String& name, Node* parent, EGEPhysics::EComponentType componentType = EGEPhysics::COMPONENT_DYNAMIC);
    virtual ~Node();

    /* Returns TRUE if object is valid. */
    bool isValid() const;
    /*! Returns node name. */
    inline const String& name() const { return m_name; }
    /*! Returns node's parent. NULL if this is root node. */
    inline Node* parent() const { return m_parent; }

    /* Deletes child node with a given name and detaches it. */
    void deleteChildNode(const String& name);
    /* Deletes and detaches all child nodes. */
    void deleteAllChildNodes();
    /* Returns number of child nodes. */
    u32 childNodeCount() const;
    /* Returns child node with a given name. Returns NULL if no such node exists. */
    Node* childNode(const String& name) const;

    /*! Returns local physics component. */
    inline PPhysicsComponent physics() const { return m_physics; }
    /*! Returns cached combined world matrix. */
    const Matrix4f& worldMatrix() const;

  protected:

    /* Creates child node with a given name and attaches it. */
    Node* createChildNode(const String& name, EGEPhysics::EComponentType componentType);
    /*! Creates child node with a given name. MUST be overriden by subclass. */
    virtual Node* createChildNodeImpl(const String& name, EGEPhysics::EComponentType componentType) = 0;

  protected:

    /*! Name. */
    String m_name;
    /*! Raw pointer to parent node. NULL if this is root node. Pointer is RAW as this object is not intended (TAGE for now ?) to be shared. */
    Node* m_parent;
    /*! List of all child nodes attached. */
    List<Node*> m_children;
    /*! Physics component. */
    PPhysicsComponent m_physics;
    /*! Cached combined world matrix from all self and all parent nodes. */
    mutable Matrix4f m_worldMatrix;
    /*! Visibility flag. */
    bool m_visible;

  private slots:

    /* Called when one of transformation values has beeen changed. */
    void transformationChanged();

  private:

    /*! Flag indicating if world matrix is invalid. */
    mutable bool m_worldMatrixInvalid;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCENENODE_H