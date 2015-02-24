#include "Core/Data/Interface/Node.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Node::Node(const String& name, Node* parent) : m_name(name)
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
  for (List<Node*>::Iterator iter = m_children.begin(); iter != m_children.end(); ++iter)
  {
    Node* node = *iter;

    // check if found
    if (node->name() == name)
    {
      // delete node
      EGE_DELETE(node);

      // remove from vector
      m_children.remove(iter);

      // we are done
      return;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
u32 Node::childNodeCount() const
{
  return static_cast<u32>(m_children.length());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Node* Node::childNode(const String& name) const
{
  // go thru all child nodes
  for (List<Node*>::ConstIterator iter = m_children.begin(); iter != m_children.end(); ++iter)
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
  for (List<Node*>::Iterator iter = m_children.begin(); iter != m_children.end();)
  {
    Node* node = *iter;

    EGE_DELETE(node);

    iter = m_children.remove(iter);
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

EGE_NAMESPACE_END