#include "Core/Overlay/OverlayManager.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(OverlayManager)
EGE_DEFINE_DELETE_OPERATORS(OverlayManager)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OverlayManager::OverlayManager() : Object()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OverlayManager::~OverlayManager()
{
  removeAll();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult OverlayManager::construct()
{
  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void OverlayManager::update(const Time& time)
{
  // update all overlays
  for (OverlayList::iterator it = m_overlays.begin(); it != m_overlays.end(); ++it)
  {
    Overlay* object = *it;

    object->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult OverlayManager::add(const POverlay& overlay)
{
  // check if overlay with given name exists
  if (this->overlay(overlay->name()))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // add to pool
  m_overlays.push_back(overlay);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void OverlayManager::remove(const String& name)
{
  // go thru all overlays
  for (OverlayList::iterator it = m_overlays.begin(); it != m_overlays.end(); ++it)
  {
    POverlay object = *it;

    // check if found
    if (object->name() == name)
    {
      object = NULL;
      m_overlays.erase(it);
      break;
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void OverlayManager::remove(const POverlay& overlay)
{
  m_overlays.remove(overlay);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void OverlayManager::removeAll()
{
  m_overlays.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
POverlay OverlayManager::overlay(const String& name) const
{
  // go thru all overlays
  for (OverlayList::const_iterator it = m_overlays.begin(); it != m_overlays.end(); ++it)
  {
    Overlay* object = *it;

    // check if found
    if (object->name() == name)
    {
      // found
      return object;
    }
  }

  return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void OverlayManager::render(Viewport* viewport, IRenderer* renderer)
{
  // go thru all overlays
  for (OverlayList::const_iterator it = m_overlays.begin(); it != m_overlays.end(); ++it)
  {
    Overlay* object = *it;

    // render
    object->addForRendering(renderer);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END