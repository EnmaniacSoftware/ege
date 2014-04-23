#include "Core/Data/Interface/Node.h"
#include "EGEDebug.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Node::Node(Application* app, const String& name, Node* parent) : m_name(name)
                                                               , m_parent(parent)
                                                               , m_visible(true)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Node::~Node()
{
  deleteAllChildNodes();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
const String& Node::name() const 
{ 
  return m_name; 
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Node* Node::parent() const 
{ 
  return m_parent; 
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
bool Node::isVisible() const 
{ 
  return m_visible; 
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
