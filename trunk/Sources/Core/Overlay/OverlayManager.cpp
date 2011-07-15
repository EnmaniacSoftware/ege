#include "Core/Overlay/OverlayManager.h"

EGE_NAMESPACE

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_DEFINE_NEW_OPERATORS(OverlayManager)
EGE_DEFINE_DELETE_OPERATORS(OverlayManager)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OverlayManager::OverlayManager(Application* app) : Object(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
OverlayManager::~OverlayManager()
{
  removeAllOverlays();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Updates manager. */
void OverlayManager::update(const Time& time)
{
  // update all overlays
  for (List<POverlay>::iterator it = m_overlays.begin(); it != m_overlays.end(); ++it)
  {
    Overlay* object = *it;

    object->update(time);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Adds text overlay of the given name. */
PTextOverlay OverlayManager::addTextOverlay(const String& name)
{
  // check if overlay with given name exists
  if (overlay(name))
  {
    // error!
    return NULL;
  }

  PTextOverlay textOverlay = ege_new TextOverlay(app(), name);
  if (textOverlay)
  {
    m_overlays.push_back(textOverlay);
  }

  return textOverlay;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Adds image overlay of the given name. */
PImageOverlay OverlayManager::addImageOverlay(const String& name)
{
  // check if overlay with given name exists
  if (overlay(name))
  {
    // error!
    return NULL;
  }

  PImageOverlay imageOverlay = ege_new ImageOverlay(app(), name);
  if (imageOverlay)
  {
    m_overlays.push_back(imageOverlay);
  }

  return imageOverlay;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Removes overlay of the given name. */
void OverlayManager::removeOverlay(const String& name)
{
  // go thru all overlays
  for (List<POverlay>::iterator it = m_overlays.begin(); it != m_overlays.end(); ++it)
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
/* Removes all overlays. */
void OverlayManager::removeAllOverlays()
{
  m_overlays.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/* Returns overlay of the given name. */
POverlay OverlayManager::overlay(const String& name) const
{
  // go thru all overlays
  for (List<POverlay>::const_iterator it = m_overlays.begin(); it != m_overlays.end(); ++it)
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
/*! Renders all elements. */
void OverlayManager::render(Viewport* viewport, Renderer* renderer)
{
//  renderer->setProjectionMatrix(
  // go thru all overlays
  for (List<POverlay>::const_iterator it = m_overlays.begin(); it != m_overlays.end(); ++it)
  {
    Overlay* object = *it;

    // render
    object->render(viewport, renderer);
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
