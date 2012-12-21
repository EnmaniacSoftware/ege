#include "Core/UI/WidgetFactory.h"
#include <EGEDebug.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(WidgetFactory)
EGE_DEFINE_DELETE_OPERATORS(WidgetFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct BuiltInWidget
{
  const char* name;
  egeWidgetCreateFunc pfCreateFunc;
};

static BuiltInWidget l_widgetsToRegister[] = {  { "dialog", Dialog::Create },
                                                { "push-button", PushButton::Create },
                                                { "scrollable-view", UIScrollableView::Create },
                                                { "label", Label::Create },
                                                { "scrollable-page-view", UIScrollablePageView::Create }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WidgetFactory::WidgetFactory(Application* app) : Object(app)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WidgetFactory::~WidgetFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult WidgetFactory::construct()
{
  EGEResult result;

  // register build-in widget types
  for (u32 i = 0; i < sizeof (l_widgetsToRegister) / sizeof (BuiltInWidget); ++i)
  {
    const BuiltInWidget& widget = l_widgetsToRegister[i];

    if (EGE_SUCCESS != (result = registerWidget(widget.name, widget.pfCreateFunc)))
    {
      // error!
      return result;
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult WidgetFactory::registerWidget(const String& typeName, egeWidgetCreateFunc createFunc)
{
  EGEResult result = EGE_SUCCESS;
  
  // check if resource with such a name exists already
  if (isWidgetRegistered(typeName))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // register
  WidgetRegistryEntry entry;
  entry.createFunc  = createFunc;

  m_registeredWidgets.insert(typeName, entry);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PWidget WidgetFactory::createWidget(const String& typeName, const String& name) const
{
  PWidget widget;

  // file emitter with given type name
  WidgetRegisterMap::const_iterator it = m_registeredWidgets.find(typeName);
  if (it != m_registeredWidgets.end())
  {
    // create resource
    widget = it->second.createFunc(app(), name);
  }

  return widget;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool WidgetFactory::isWidgetRegistered(const String& typeName) const
{
  return m_registeredWidgets.contains(typeName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END