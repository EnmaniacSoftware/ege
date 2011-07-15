#include "Core/Data/Node.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Node::Node(Application* app, const String& name, Node* parent, EGEPhysics::EComponentType componentType) : m_name(name), m_parent(parent)
{
  m_physics = ege_new PhysicsComponent(app, "node-physics-" + name, componentType);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Node::~Node()
{
  deleteAllChildNodes();

  m_physics = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns TRUE if object is valid. */
bool Node::isValid() const
{
  return (NULL != m_physics) && m_physics->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Creates child node with a given name and attaches it. */
Node* Node::createChildNode(const String& name, EGEPhysics::EComponentType componentType)
{
  Node* node;

  // check if there is already node with the given name
  if (NULL != childNode(name))
  {
    // cannot create
    return NULL;
  }

  // create node
  node = createChildNodeImpl(name, componentType);
  if (NULL != node && node->isValid())
  {
    // add into vector
    m_children.push_back(node);
  }
  else
  {
    // error!
    EGE_DELETE(node);
  }

  return node;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Deletes child node with a given name and detaches it. */
void Node::deleteChildNode(const String& name)
{
  // go thru all child nodes
  for (List<Node*>::iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
  {
    Node* node = *iter;

    // check if found
    if (node->name() == name)
    {
      // delete node
      EGE_DELETE(node);

      // remove from vector
      m_children.erase(iter);

      // we are done
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns number of child nodes. */
u32 Node::childNodeCount() const
{
  return static_cast<u32>(m_children.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns child node with a given name. Returns NULL if no such node exists. */
Node* Node::childNode(const String& name) const
{
  // go thru all child nodes
  for (List<Node*>::const_iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
  {
    Node* node = *iter;

    // check if found
    if (node->name() == name)
    {
      // found
      return node;
    }
  }

  // not found
  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Returns child node with a given index. Returns NULL if no such node exists. */
//Node* Node::childNode(u32 index) const
//{
//  // check if OUT of range
//  if (index >= childNodeCount())
//  {
//    // error!
//    return NULL;
//  }
//
//  return m_children[index];
//}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Deletes and detaches all child nodes. */
void Node::deleteAllChildNodes()
{
  for (List<Node*>::iterator iter = m_children.begin(); iter != m_children.end();)
  {
    Node* node = *iter;

    EGE_DELETE(node);

    iter = m_children.erase(iter);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------