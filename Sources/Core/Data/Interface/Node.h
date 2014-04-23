#ifndef EGE_CORE_DATA_NODE_H
#define EGE_CORE_DATA_NODE_H

/** Class representing a general-purpose node in arbitrary graph.
 */

#include "EGE.h"
#include "EGEList.h"
#include "EGEString.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Application;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Node
{
  public:

    Node(Application* app, const String& name, Node* parent);
    virtual ~Node();

    /*! Returns node name. */
    const String& name() const;

    /*! Returns node's parent. NULL if this is root node. */
    Node* parent() const;

    /*! Deletes child node with a given name and detaches it. 
     *  @param  name  Name of the child node to delete.
     */
    void deleteChildNode(const String& name);
    /*! Deletes and detaches all child nodes. */
    void deleteAllChildNodes();
    /*! Returns number of child nodes. */
    u32 childNodeCount() const;
    /*! Returns child node with a given name.
     *  @param name Name of the child not to find.
     *  @return Pointer to child node with the given name or NULL if not found.
     */
    Node* childNode(const String& name) const;

    /*! Returns TRUE if object is visible. */
    bool isVisible() const;
    /*! Sets visibility flag. 
     *  @param  set TRUE if node should become visible. Otherwise, FALSE.
     */
    void setVisible(bool set);

  protected:

    /*! Name. */
    String m_name;
    /*! Raw pointer to parent node. NULL if this is root node. Pointer is RAW as this object is not intended (TAGE for now ?) to be shared. */
    Node* m_parent;
    /*! List of all child nodes attached. */
    List<Node*> m_children;
    /*! Visibility flag. */
    bool m_visible;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_DATA_NODE_H