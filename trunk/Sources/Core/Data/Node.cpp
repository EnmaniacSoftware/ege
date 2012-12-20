#include "Core/Data/Node.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Node::Node(Application* app, const String& name, Node* parent, EGEPhysics::ComponentType componentType) : m_name(name), 
                                                                                                          m_parent(parent), 
                                                                                                          m_visible(true),
                                                                                                          m_worldMatrixInvalid(true)
{
  m_physics = ege_new PhysicsComponent(app, "node-physics-" + name, componentType);
  if (m_physics)
  {
    ege_connect(m_physics, transformationChanged, this, Node::transformationChanged);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Node::~Node()
{
  deleteAllChildNodes();

  m_physics = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Node::isValid() const
{
  return (NULL != m_physics) && m_physics->isValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Node* Node::createChildNode(const String& name, EGEPhysics::ComponentType componentType)
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
u32 Node::childNodeCount() const
{
  return static_cast<u32>(m_children.size());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
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
const Matrix4f& Node::worldMatrix() const 
{ 
  if (m_worldMatrixInvalid)
  {
    Quaternionf orientation = physics()->orientation();
    Vector4f position = physics()->position();
    Vector4f scale = physics()->scale();
  
    Math::CreateMatrix(&m_worldMatrix, &position, &scale, &orientation);
  
    if (NULL != parent())
    {
      m_worldMatrix = parent()->worldMatrix().multiply(m_worldMatrix);
    }

    // validate
    m_worldMatrixInvalid = false;
  }

  return m_worldMatrix; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Node::transformationChanged()
{
  // invalidate world matrix
  m_worldMatrixInvalid = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void Node::setVisible(bool set)
{
  if (isVisible() != set)
  {
    m_visible = set;
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END